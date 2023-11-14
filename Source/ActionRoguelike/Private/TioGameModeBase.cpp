// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/TioAICharacter.h"
#include "TioAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TioCharacter.h"
#include "TioPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TioSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "TioGameplayInterface.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tio.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDrawFailedPickupLocation(TEXT("tio.FailedPickupLocation"), true, TEXT("DrawFailedPickupLocation"), ECVF_Cheat);

ATioGameModeBase::ATioGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	/*for power-up actors*/
	DesiredPowerupCount = 5;
	RequiredPowerupDistance = 500;

	PlayerStateClass = ATioPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ATioGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ATioGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATioGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATioGameModeBase::OnPowerupQueryComplete);
		}
	}
}

void ATioGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 96节课
	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	ATioPlayerState* Ps = NewPlayer->GetPlayerState<ATioPlayerState>();
	if (Ps)
	{
		Ps->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ATioGameModeBase::OnPowerupQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerup EQS query failed"));
		return;
	}
	bool bDraw = CVarDrawFailedPickupLocation.GetValueOnGameThread();

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	/*Keep used locations to easily check distance between points*/
	TArray<FVector> UsedLocations;

	int32 SpawnConunter = 0;
	/*Break out if we reached the desired count or if we have no more potential position remaining*/
	while (SpawnConunter < DesiredPowerupCount && Locations.Num() > 0)
	{
		/*Pick a random location from remaining points*/
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		/*Check minimum distance requirement*/
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerupDistance)
			{
				/*show skipped location*/
				if (bDraw)
				{
					DrawDebugSphere(GetWorld(), OtherLocation, 5.0f, 32, FColor::Red, false, 10.0f);
				}

				/*Too close ,skip to next one*/
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}

		/*pick a random power-up class*/
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		AActor* NewObj = GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);
		UsedLocations.Add(PickedLocation);
		++SpawnConunter;
	}
}

void ATioGameModeBase::KillAll()
{
	for (TActorIterator<ATioAICharacter> It(GetWorld()); It; ++It)
	{
		ATioAICharacter* Bot = *It;
		
		UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void ATioGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bot spawning disable via 'CVarSpawnBots'."));
		return;
	}

	int32 NrofAliveBots = 0;
	// check all <instances of that class> in current level
	for (TActorIterator<ATioAICharacter> It(GetWorld()); It; ++It)
	{
		ATioAICharacter* Bot = *It;

		UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			++NrofAliveBots;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrofAliveBots);

	float MaxBotCount = 0.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	// early out if we can't spawn any bots
	if (NrofAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capcity. Skipping bot spawn."));
		return;
	}

	// quite expensive
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATioGameModeBase::OnQueryComplete);
	}
	
}

void ATioGameModeBase::OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);

		UE_LOG(LogTemp, Log, TEXT("Bot spawned"));
	}
}

void ATioGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ATioGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ATioCharacter* Player = Cast<ATioCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ATioPlayerState* PS = KillerPawn->GetPlayerState<ATioPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}


void ATioGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ATioPlayerState* Ps = Cast<ATioPlayerState>(GameState->PlayerArray[i]);
		if (Ps)
		{
			Ps->SavePlayerState(CurrentSaveGame);
			break; // Single player game at this time
		}
	}

	// clear saved actors, otherwise duplicate saves will be made
	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UTioGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's UPROPERTY(SaveGame) variables into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ATioGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UTioSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("unable to load saved game."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Game Loaded."));

		// Iterator the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// only interested in our 'gameplay actors'
			if (!Actor->Implements<UTioGameplayInterface>())
			{
				continue;
			}
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (Actor->GetName() == ActorData.ActorName)
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;

					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					ITioGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UTioSaveGame>(UGameplayStatics::CreateSaveGameObject(UTioSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}

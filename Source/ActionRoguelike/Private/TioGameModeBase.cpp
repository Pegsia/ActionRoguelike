// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/TioAICharacter.h"
#include "TioAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

ATioGameModeBase::ATioGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ATioGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATioGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ATioGameModeBase::SpawnBotTimerElapsed()
{
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

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrofAliveBots);

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


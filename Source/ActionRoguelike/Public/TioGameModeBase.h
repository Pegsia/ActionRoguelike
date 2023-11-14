// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TioGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UTioSaveGame;

UCLASS()
class ACTIONROGUELIKE_API ATioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY()
	UTioSaveGame* CurrentSaveGame;

	/*Distance required between power-up spawn locations*/
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float RequiredPowerupDistance;

	/*Amount of power-up to spawn during match start*/
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 DesiredPowerupCount;

	/*All power-up classes used to spawn with EQS*/
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	UEnvQuery* PowerupQuery;

	// Minion/AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void OnPowerupQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	// The GameMode's InitGame() event is called before any other functions(including PreInitializeComponents())
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Signals that a player is ready to enter the game, which may start it up */
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();
	
	ATioGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

};

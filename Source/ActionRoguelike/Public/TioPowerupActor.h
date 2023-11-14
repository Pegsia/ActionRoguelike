// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ATioPowerupActor : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsAcitve;

	UFUNCTION()
	void OnRep_IsActive();

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	ATioPowerupActor();

};

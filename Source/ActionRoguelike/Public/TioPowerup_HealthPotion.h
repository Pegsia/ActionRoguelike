// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioPowerupActor.h"
#include "TioPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATioPowerup_HealthPotion : public ATioPowerupActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Powerup")
	int32 CreditsCost;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float HealAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	ATioPowerup_HealthPotion();
};

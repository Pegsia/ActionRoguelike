// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioPowerupActor.h"
#include "TioPowerup_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATioPowerup_Credits : public ATioPowerupActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
	int32 CreditsAmount;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

	ATioPowerup_Credits();
};

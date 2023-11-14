// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioPowerupActor.h"
#include "TioPowerup_Action.generated.h"

class UTioAction;

UCLASS()
class ACTIONROGUELIKE_API ATioPowerup_Action : public ATioPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UTioAction> ActionToGrant;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;
};

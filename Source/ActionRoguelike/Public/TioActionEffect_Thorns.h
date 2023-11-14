// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioActionEffect.h"
#include "TioActionEffect_Thorns.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UTioActionEffect_Thorns : public UTioActionEffect
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UTioActionEffect_Thorns();
};

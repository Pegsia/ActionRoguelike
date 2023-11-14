// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioAction.h"
#include "TioActionEffect.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UTioActionEffect : public UTioAction
{
	GENERATED_BODY()
	
public:
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:
	/*Buffs duration time*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/*Buffs trigger time*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);	

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

	UTioActionEffect();
};

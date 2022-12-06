// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TioProjectileBase.h"
#include "TioMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATioMagicProjectile : public ATioProjectileBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;

public:
	// Sets default values for this actor's properties
	ATioMagicProjectile();
};



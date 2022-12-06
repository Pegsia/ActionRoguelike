// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioProjectileBase.generated.h"

class USoundCue;
class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(ABSTRACT) // 标志这个类还没有完成，引擎中不可见，但可以继承
class ACTIONROGUELIKE_API ATioProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent *SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;


	// virtual so we can override this in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = C++ base implementation, can be expanded in blueprints
	// BluePrintCallable to allow child classes to trigger explosions
	// Not required for assignment, useful for expanding in Blueprint later on
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();

	virtual void PostInitializeComponents() override;
public:	
	// Sets default values for this actor's properties
	ATioProjectileBase();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTioInteractionComponent;
class UTioAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ATioCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor>	ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimeDelay;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_Dash;

public:
	// Sets default values for this character's properties
	ATioCharacter();

protected:
	// start with U is unreal macros
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UTioInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTioAttributeComponent* AttributeComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();

	void Dash();
	void Dash_TimeElapsed();

	void StartAttackEffects();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwninComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

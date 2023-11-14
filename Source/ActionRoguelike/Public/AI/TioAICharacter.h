// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioAICharacter.generated.h"

class UPawnSensingComponent;
class UTioAttributeComponent;
class UUserWidget;
class UTioWorldUserWidget;
class UTioActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ATioAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATioAICharacter();

protected:

	UTioWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	void SetTargetActor(AActor* NewTarget);

	AActor* GetTargetActor() const;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Compoents")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();
};

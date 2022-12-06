// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioAICharacter.generated.h"

class UPawnSensingComponent;
class UTioAttributeComponent;
class UUserWidget;
class UTioWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ATioAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATioAICharacter();

protected:

	UTioWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Compoents")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};

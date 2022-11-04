// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioTargetDummy.generated.h"

class UTioAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ATioTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATioTargetDummy();

protected:
	
	UPROPERTY(VisibleAnywhere)
	UTioAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwninComp, float NewHealth, float Delta);
};

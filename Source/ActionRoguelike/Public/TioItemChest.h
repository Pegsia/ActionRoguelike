// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATioItemChest : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	ATioItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GoldPileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* Burst;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioDoor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATioDoor : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATioDoor();

	UPROPERTY(EditAnywhere)
	float TargetYaw;

	void Interact_Implementation(APawn* InstigatorPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FrameMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

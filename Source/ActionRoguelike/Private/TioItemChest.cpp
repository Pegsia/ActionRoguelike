// Fill out your copyright notice in the Description page of Project Settings.


#include "TioItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATioItemChest::ATioItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	GoldPileMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldPieMesh");
	GoldPileMesh->SetupAttachment(BaseMesh);

	Burst = CreateDefaultSubobject<UParticleSystemComponent>("Burst");
	Burst->SetupAttachment(GoldPileMesh);
	Burst->bAutoActivate = false;

	TargetPitch = 110;
}

void ATioItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}


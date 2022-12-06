// Fill out your copyright notice in the Description page of Project Settings.


#include "TioDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATioDoor::ATioDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	RootComponent = FrameMesh;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	TargetYaw = 90;

}

void ATioDoor::Interact_Implementation(APawn* InstigatorPawn)
{
	DoorMesh->SetRelativeRotation(FRotator(0, TargetYaw, 0));
}

// Called when the game starts or when spawned
void ATioDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATioDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


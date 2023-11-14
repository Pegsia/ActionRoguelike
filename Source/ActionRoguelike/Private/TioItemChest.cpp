// Fill out your copyright notice in the Description page of Project Settings.


#include "TioItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);
}

void ATioItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened; // 来回开关
	OnRep_LidOpened();        // bLidOpened变化后不会在server端自动触发OnRep函数
}

void ATioItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ATioItemChest::OnRep_LidOpened()
{
	float NextPitch = bLidOpened ? TargetPitch : 0.0f; // 先变化再触发这个函数，所以条件是反着的
	LidMesh->SetRelativeRotation(FRotator(NextPitch, 0, 0));
}

void ATioItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATioItemChest, bLidOpened);
}
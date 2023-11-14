// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATioPowerupActor::ATioPowerupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Powerup");
    RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

    RespawnTime = 10.0f;

    SetReplicates(true);

    bIsAcitve = true;
}

void ATioPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
    // logic in derived class
}

void ATioPowerupActor::ShowPowerup()
{
    SetPowerupState(true);
}

void ATioPowerupActor::HideAndCooldownPowerup()
{
    SetPowerupState(false);

    FTimerHandle TimerHandle_RespawnTimer;
    GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ATioPowerupActor::ShowPowerup, RespawnTime);
}

void ATioPowerupActor::SetPowerupState(bool bNweIsActive)
{
    bIsAcitve = bNweIsActive;
    OnRep_IsActive();
}

void ATioPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsAcitve);

	// 第二个参数控制显示是否传递到孩子节点
	RootComponent->SetVisibility(bIsAcitve, true);
}

void ATioPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATioPowerupActor, bIsAcitve);
}
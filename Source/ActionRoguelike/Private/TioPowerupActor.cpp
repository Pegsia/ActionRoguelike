// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATioPowerupActor::ATioPowerupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Powerup");
    RootComponent = SphereComp;

    RespawnTime = 10.0f;
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
    SetActorEnableCollision(bNweIsActive);

    // 第二个参数控制显示是否传递到孩子节点
    RootComponent->SetVisibility(bNweIsActive, true);
}



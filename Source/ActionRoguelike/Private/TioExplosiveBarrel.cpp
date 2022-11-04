// Fill out your copyright notice in the Description page of Project Settings.


#include "TioExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATioExplosiveBarrel::ATioExplosiveBarrel()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionProfileName("PhysicsActor");
    RootComponent = MeshComp;

    ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
    ForceComp->SetupAttachment(MeshComp);

    ForceComp->SetAutoActivate(false);

    ForceComp->Radius = 750.0f;
    ForceComp->ImpulseStrength = 1000.0f;
    ForceComp->bImpulseVelChange = true;

    ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ATioExplosiveBarrel::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    MeshComp->OnComponentHit.AddDynamic(this, &ATioExplosiveBarrel::OnActorHit);
}

void ATioExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ForceComp->FireImpulse();
    UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

    UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

    FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
    DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}




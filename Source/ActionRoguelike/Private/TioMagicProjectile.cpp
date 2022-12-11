// Fill out your copyright notice in the Description page of Project Settings.


#include "TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TioAttributeComponent.h"
#include "TioGameplayFunctionLibrary.h"

// Sets default values
ATioMagicProjectile::ATioMagicProjectile()
{
	DamageAmount = 20.0f;
	MoveComp->InitialSpeed = 2000;
}

void ATioMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATioMagicProjectile::OnActorOverlap);
}

void ATioMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(OtherActor);
		//// DeBug时候没有反应是因为编译器进行了优化把Development Editor改成DebugGame就行了
		//if (AttributeComp)
		//{
		//	AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

		//	Explode();
		//}
		if (UTioGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}



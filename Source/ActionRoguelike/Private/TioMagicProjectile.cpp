// Fill out your copyright notice in the Description page of Project Settings.


#include "TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TioAttributeComponent.h"
#include "TioGameplayFunctionLibrary.h"
#include "TioActionComponent.h"
#include "TioActionEffect.h"

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
		UTioActionComponent* ActionComp = Cast<UTioActionComponent>(OtherActor->GetComponentByClass(UTioActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			// 如果被击中用户可以回避炮弹，那么将炮弹反向，并将炮弹的insigator设为用户并且返回
			MoveComp->Velocity = -MoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UTioGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
			// AddAction是关于GameState的操作，故只应在服务器上运行
			if (ActionComp && HasAuthority() && EffectActionClass) //add effect action if the projectile have
			{
				ActionComp->AddAction(GetInstigator(), EffectActionClass);
			}
		}
	}
}



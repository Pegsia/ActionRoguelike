// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameplayFunctionLibrary.h"
#include <TioAttributeComponent.h>

static TAutoConsoleVariable<float> CVarMImpulseStrength(TEXT("tio.MImpulseStrength"), 3000.f, TEXT("Change Minion Impulse Strength."), ECVF_Cheat);

bool UTioGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UTioGameplayFunctionLibrary::ApplyDirectionDamage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamgeCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * CVarMImpulseStrength.GetValueOnGameThread(), HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}


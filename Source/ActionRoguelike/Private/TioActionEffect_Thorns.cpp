// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionEffect_Thorns.h"
#include "TioAttributeComponent.h"
#include "TioActionComponent.h"
#include "TioGameplayFunctionLibrary.h"

UTioActionEffect_Thorns::UTioActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UTioActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UTioActionEffect_Thorns::OnHealthChanged);
	}
}

void UTioActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UTioActionEffect_Thorns::OnHealthChanged);
	}
}

void UTioActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta < 0.0f && InstigatorActor != OwningActor)
	{
		int32 ReflectAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectAmount = FMath::Abs(ReflectAmount);

		// Return damage sender...
		UTioGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectAmount);
	}
}


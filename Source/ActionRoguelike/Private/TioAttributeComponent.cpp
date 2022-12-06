// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAttributeComponent.h"

// Sets default values for this component's properties
UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}


bool UTioAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UTioAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UTioAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return ActualDelta != 0;
}

UTioAttributeComponent* UTioAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UTioAttributeComponent>(FromActor->GetComponentByClass(UTioAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UTioAttributeComponent::IsActorAlive(AActor* Actor)
{
	UTioAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}


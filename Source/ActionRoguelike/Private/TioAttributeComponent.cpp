// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAttributeComponent.h"
#include "TioWorldUserWidget.h"
#include "AI/TioAICharacter.h"
#include "TioGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tio.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UTioAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UTioAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UTioAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UTioAttributeComponent::GetHealth() const
{
	return Health;
}

float UTioAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		// GameMode only on Server and Auth is sense for authority which is the server
		ATioGameModeBase* GM = GetWorld()->GetAuthGameMode<ATioGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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


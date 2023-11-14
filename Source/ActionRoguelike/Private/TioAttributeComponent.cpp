// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAttributeComponent.h"
#include "TioWorldUserWidget.h"
#include "AI/TioAICharacter.h"
#include "TioGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tio.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	// 只在server上改变健康数据，同时保持ActualDelta的计算用来返回该角色是否可被伤害
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth; // Health改变后，自动传播给客户端

		if (ActualDelta != 0.0f) // 对于多人游戏，尽可能的减少传播
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		if (ActualDelta < 0.0f && Health == 0.0f) // Died，只有server上存在GameMode，所以不用在client上检查该项
		{
			// GameMode only on Server and Auth is sense for authority which is the server
			ATioGameModeBase* GM = GetWorld()->GetAuthGameMode<ATioGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

float UTioAttributeComponent::GetRage() const
{
	return Rage;
}

bool UTioAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = Rage - OldRage;

	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
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

void UTioAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta);
}

void UTioAttributeComponent::MulticastRageChanged_Implementation(AActor* Instigator, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(Instigator, this, NewRage, Delta);
}

void UTioAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTioAttributeComponent, Health);
	DOREPLIFETIME(UTioAttributeComponent, HealthMax);

	DOREPLIFETIME(UTioAttributeComponent, Rage);
	DOREPLIFETIME(UTioAttributeComponent, RageMax);
}
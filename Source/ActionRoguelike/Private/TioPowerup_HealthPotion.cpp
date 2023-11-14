// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerup_HealthPotion.h"
#include "TioAttributeComponent.h"
#include "TioPlayerState.h"

ATioPowerup_HealthPotion::ATioPowerup_HealthPotion()
{
	HealAmount = 40.0f;

	CreditsCost = 50;
}

void ATioPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(InstigatorPawn);
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ATioPlayerState* PS = InstigatorPawn->GetPlayerState<ATioPlayerState>())
		{
			if (PS->RemoveCredits(CreditsCost) && AttributeComp->ApplyHealthChange(this, HealAmount))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}


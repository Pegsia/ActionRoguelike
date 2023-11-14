// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerup_Action.h"
#include "TioActionComponent.h"
#include "TioAction.h"

void ATioPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UTioActionComponent* ActionCopm = Cast<UTioActionComponent>(InstigatorPawn->GetComponentByClass(UTioActionComponent::StaticClass()));
	if (ActionCopm)
	{
		if (ActionCopm->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new Ability
		ActionCopm->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}

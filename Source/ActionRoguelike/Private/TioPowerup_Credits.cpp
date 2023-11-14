// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerup_Credits.h"
#include "TioPlayerState.h"

ATioPowerup_Credits::ATioPowerup_Credits()
{
	CreditsAmount = 100;
}

void ATioPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ATioPlayerState* PS = InstigatorPawn->GetPlayerState<ATioPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}

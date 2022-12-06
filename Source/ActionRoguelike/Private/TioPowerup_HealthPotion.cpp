// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerup_HealthPotion.h"
#include "TioAttributeComponent.h"

ATioPowerup_HealthPotion::ATioPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	HealAmount = 40.0f;
}

void ATioPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UTioAttributeComponent* AttributeComp = Cast<UTioAttributeComponent>(InstigatorPawn->GetComponentByClass(UTioAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AttributeComp->ApplyHealthChange(this, HealAmount))
		{
			HideAndCooldownPowerup();
		}
	}
}


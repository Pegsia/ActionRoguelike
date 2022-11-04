// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAttributeComponent.h"

// Sets default values for this component's properties
UTioAttributeComponent::UTioAttributeComponent()
{
	Health = 100;
}


bool UTioAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UTioAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}



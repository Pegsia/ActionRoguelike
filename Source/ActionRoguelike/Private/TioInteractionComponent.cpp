// Fill out your copyright notice in the Description page of Project Settings.


#include "TioInteractionComponent.h"
#include "TioGameplayInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVArDebugDrawInteraction(TEXT("tio.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Compoent."), ECVF_Cheat);

// Sets default values for this component's properties
UTioInteractionComponent::UTioInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTioInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTioInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTioInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVArDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 500);

	/*FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ?  FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTioGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ITioGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}


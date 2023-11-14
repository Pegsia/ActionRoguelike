// Fill out your copyright notice in the Description page of Project Settings.


#include "TioInteractionComponent.h"
#include "TioGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "TioWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVArDebugDrawInteraction(TEXT("tio.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Compoent."), ECVF_Cheat);

UTioInteractionComponent::UTioInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;

	TraceInterval = 0.06f;
}

void UTioInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	/*APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner->IsLocallyControlled())
	{
		GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_TraceActor, this, &UTioInteractionComponent::FindBestInteractable, TraceInterval, true);
	}*/
}

void UTioInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// try to set a timer rather than run every tick
	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UTioInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVArDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	// clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTioGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	// set display widget when actor can be interacted
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UTioWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance) 
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport()) // if widget is not in viewport yet
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}


void UTioInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UTioInteractionComponent::ServerInteract_Implementation(AActor* InFocuseActor)
{
	if (InFocuseActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ITioGameplayInterface::Execute_Interact(InFocuseActor, MyPawn);
}


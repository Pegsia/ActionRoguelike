// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioInteractionComponent.generated.h"

class UTioWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTioInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocuseActor);

	void PrimaryInteract();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceInterval;

	FTimerHandle TimerHandle_TraceActor;

	UPROPERTY()
	UTioWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTioWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	AActor* FocusedActor;

	void FindBestInteractable();

	virtual void BeginPlay() override;

public:	

	UTioInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

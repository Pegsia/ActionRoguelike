// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPlayerController.h"
#include "Blueprint/UserWidget.h"

void ATioPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100); // above all

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ATioPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ATioPlayerController::TogglePauseMenu);
}

void ATioPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ATioPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ATioPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

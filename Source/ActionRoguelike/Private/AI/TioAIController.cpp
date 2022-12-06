// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TioAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATioAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	
	
	// WorldContextObject 描述为在游戏世界中的物品或controller
	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		SetFocus(MyPawn);

		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}*/

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TioBTTask_HealSelf.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTioBTTask_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UTioBTTask_HealSelf();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float HealAmount;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TioBTTask_HealSelf.h"
#include "AIController.h"
#include "TioAttributeComponent.h"

UTioBTTask_HealSelf::UTioBTTask_HealSelf()
{
	HealAmount = 40.0f;
}

EBTNodeResult::Type UTioBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(AIPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AIPawn, HealAmount);
	}
	return EBTNodeResult::Succeeded;
}

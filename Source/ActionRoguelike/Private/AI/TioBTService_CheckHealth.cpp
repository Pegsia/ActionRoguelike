// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TioBTService_CheckHealth.h"
#include "AIController.h"
#include "TioAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UTioBTService_CheckHealth::UTioBTService_CheckHealth()
{
	LowHealthFraction = 0.4f;
}

void UTioBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn)
	{
		UTioAttributeComponent* AttributeComp = UTioAttributeComponent::GetAttributes(AIPawn);
		if (AttributeComp)
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) <= LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}

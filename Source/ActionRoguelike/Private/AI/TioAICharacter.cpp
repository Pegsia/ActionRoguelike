// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TioAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "BrainComponent.h"
#include "TioWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TioActionComponent.h"

// Sets default values
ATioAICharacter::ATioAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComp");

    ActionComp = CreateDefaultSubobject<UTioActionComponent>("ActionComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    TimeToHitParamName = "TimeToHit";
    TargetActorKey = "TargetActor";
}

void ATioAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ATioAICharacter::OnPawnSeen);

    AttributeComp->OnHealthChanged.AddDynamic(this, &ATioAICharacter::OnHealthChanged);
}

void ATioAICharacter::OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (ActiveHealthBar == nullptr)
        {
			ActiveHealthBar = CreateWidget<UTioWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
                ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
        }
        
        //UE_LOG(LogTemp, Log, TEXT("Health %f Take %f Damage"), NewHealth, Delta);
        if (NewHealth <= 0.0f)
        {
            ActiveHealthBar->RemoveFromParent();
            //UE_LOG(LogTemp, Log, TEXT("Bot dead"));
            // stop BT
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }
            // rag doll
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");

            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            // set lifespan(how long we call destory() on ourselfs
            SetLifeSpan(10.0f);
        }
    }
}

void ATioAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);;
	}
}

AActor* ATioAICharacter::GetTargetActor() const
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    }
    return nullptr;
}

void ATioAICharacter::OnPawnSeen(APawn* Pawn)
{
    if (GetTargetActor() != Pawn)
    {
        SetTargetActor(Pawn);
        MulticastPawnSeen();
    }
    //DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ATioAICharacter::MulticastPawnSeen_Implementation()
{
	UTioWorldUserWidget* NewWidget = CreateWidget<UTioWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;

		NewWidget->AddToViewport(10);
	}
}



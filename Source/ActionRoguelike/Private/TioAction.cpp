// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAction.h"
#include "TioActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UTioAction::Initialize(UTioActionComponent* NewComp)
{
	// 该初始化只在server端运行，所以需要将其标注为replicated以让客户端也可以正确拥有该行为
	ActionComp = NewComp; 
}

bool UTioAction::CanStart_Implementation(AActor* Insitgator)
{
	if (IsRunning())
	{
		return false;
	}

	UTioActionComponent* OwningComp = GetOwningComponent();
	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

 void UTioAction::StartAction_Implementation(AActor* Instigator)
 {
 	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
 
	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	TimeStarted = GetWorld()->TimeSeconds;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
 }

 void UTioAction::StopAction_Implementation(AActor* Instigator)
 {
 	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	//ensureAlways(bIsRunning); 发送RPC后server的bIsRunning先变化再调用，所以client上永远是反着的

	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
 }

 UWorld* UTioAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T> in TioActionComponent.cpp
	// 当UObject被serve复制到client后，会自动将Outer设置为Character这里就会报错
	// UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	AActor* MyActor = Cast<AActor>(GetOuter());
	if (MyActor)
	{
		return MyActor->GetWorld();
	}
	return nullptr;
}

UTioActionComponent* UTioAction::GetOwningComponent() const
{
	return ActionComp;
}

void UTioAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator); // 注意不是蓝图Implementation版本
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UTioAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UTioAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTioAction, RepData);
	DOREPLIFETIME(UTioAction, ActionComp);
}
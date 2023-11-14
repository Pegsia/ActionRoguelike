// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "TioAction.generated.h"

class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY() // 这里不要标记为replicated
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UTioAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	UPROPERTY(Replicated)
	UTioActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTioActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer BlockedTags;

	// 确保一个action在没有开始的情况下不会调用结束
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	float TimeStarted;

public:

	void Initialize(UTioActionComponent* NewComp);

	/*start immediately when add to an action component*/
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Insitgator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable ,BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	/* Action nickname to start/stop without a reference to the object */
	/* FName is a hashed name */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override { return true; }
};

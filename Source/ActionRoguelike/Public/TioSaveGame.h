// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TioSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	/* Identifier for which Actor this belongs to */
	UPROPERTY()
	FString ActorName;

	/* For movable Actor's, keep location, rotation, scale */
	UPROPERTY()
	FTransform Transform;

	/* For variables with UPROPERTY(SaveGame), convert them to binary data and save them in ByteData array */
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class ACTIONROGUELIKE_API UTioSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "MySaveLoadGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREALEXOCPP_API UMySaveLoadGame : public USaveGame
{
	GENERATED_BODY()

public: 

	UMySaveLoadGame();

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealExoCppGameMode.h"
#include "UnrealExoCppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealExoCppGameMode::AUnrealExoCppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

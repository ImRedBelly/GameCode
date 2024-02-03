// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCodeGameMode.h"
#include "GameCodeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameCodeGameMode::AGameCodeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

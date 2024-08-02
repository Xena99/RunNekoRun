// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunNekoRunGameMode.h"
#include "RunNekoRunCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARunNekoRunGameMode::ARunNekoRunGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

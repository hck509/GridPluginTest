// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PluginTest.h"
#include "PluginTestGameMode.h"
#include "PluginTestPlayerController.h"
#include "PluginTestCharacter.h"

APluginTestGameMode::APluginTestGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APluginTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
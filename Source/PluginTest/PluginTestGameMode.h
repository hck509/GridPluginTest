// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "PluginTestGameMode.generated.h"

UCLASS(minimalapi)
class APluginTestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APluginTestGameMode(const FObjectInitializer& ObjectInitializer);
};




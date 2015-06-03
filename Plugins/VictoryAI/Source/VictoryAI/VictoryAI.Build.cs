// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VictoryAI : ModuleRules
{
	public VictoryAI(TargetInfo Target)
	{
        PublicIncludePaths.AddRange(new string[] { 
			"VictoryAI/Public"
		});

		
        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"RHI"
			}
		);
	}
}

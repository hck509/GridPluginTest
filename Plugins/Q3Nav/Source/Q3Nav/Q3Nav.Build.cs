// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Q3Nav : ModuleRules
{
	public Q3Nav(TargetInfo Target)
	{
        PublicIncludePaths.AddRange(new string[] { 
			"Q3Nav/Public"
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

// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PluginTest : ModuleRules
{
	public PluginTest(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

	    PrivateDependencyModuleNames.AddRange(new string[] { "Q3Nav" });
	    PrivateIncludePathModuleNames.AddRange(new string[] { "Q3Nav" });
	}
}

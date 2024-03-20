// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameCode : ModuleRules
{
	public GameCode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara", "UMG" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Etheria : ModuleRules
{
	public Etheria(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Slate", "SlateCore", "UMG" });

        PrivateIncludePaths.Add("Etheria");
    }
}

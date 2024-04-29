// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class unrealcourse : ModuleRules
{
	public unrealcourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "GameplayTasks", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
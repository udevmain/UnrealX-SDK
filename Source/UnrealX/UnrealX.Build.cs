// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealX : ModuleRules
{
	public UnrealX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;			
		
		PublicDependencyModuleNames.AddRange(
		new string[]
		{
			"Core",
		});
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"HTTP",
			"UnrealXMysqlLib",
			"UnrealXMysqlZLib",
			"UnrealXMysqlZstd",
			}
		);
	}
}

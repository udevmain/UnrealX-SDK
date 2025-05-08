// � 2025 UnrealX, all rights reserved by ELife Studio

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
            "Json",
			"JsonUtilities",
			"ApplicationCore", 
			"WebBrowser", 
			"EngineSettings",
            "OnlineSubsystem", "OnlineSubsystemUtils"
            }
		);
	}
}

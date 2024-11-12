using UnrealBuildTool;

public class UnrealXMysqlZLib : ModuleRules
{
	public UnrealXMysqlZLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange( new string[] { } );
		PublicDependencyModuleNames.AddRange(
			new string[]
				{
					"Core",
					"Engine",
				}
			);
		PrivateDependencyModuleNames.AddRange(new string[] { });
		DynamicallyLoadedModuleNames.AddRange(new string[] { });

		bEnableUndefinedIdentifierWarnings = false;
		bEnableExceptions = true;
		bUseRTTI = true;
		PrivateDefinitions.Add("__STDC_WANT_SECURE_LIB__=1");
		PrivateDefinitions.Add("ZLIB_DLL");
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PrivateDefinitions.Add("_WIN32");
		}
		//else if (Target.Platform == UnrealTargetPlatform.Linux)
		//{
		//}
	}
}

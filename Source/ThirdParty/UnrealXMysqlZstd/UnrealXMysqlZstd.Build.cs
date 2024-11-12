using UnrealBuildTool;

public class UnrealXMysqlZstd : ModuleRules
{
	public UnrealXMysqlZstd(ReadOnlyTargetRules Target) : base(Target)
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
		PrivateDependencyModuleNames.AddRange(
			new string[] 
				{
				}
			);
		DynamicallyLoadedModuleNames.AddRange(new string[] { });
		bEnableUndefinedIdentifierWarnings = false;
		bEnableExceptions = true;
		bUseRTTI = true;
		PrivateDefinitions.Add("__STDC_WANT_SECURE_LIB__=1");
		PrivateDefinitions.Add("DBUG_OFF");
		PrivateDefinitions.Add("NDEBUG");
		PrivateDefinitions.Add("NOGDI");
		PrivateDefinitions.Add("NOMINMAX");
		PrivateDefinitions.Add("RAPIDJSON_NO_SIZETYPEDEFINE");
		PrivateDefinitions.Add("RAPIDJSON_SCHEMA_USE_INTERNALREGEX=0");
		PrivateDefinitions.Add("RAPIDJSON_SCHEMA_USE_STDREGEX=1");
		PrivateDefinitions.Add("HAVE_CONFIG_H");
		PrivateDefinitions.Add("__STDC_FORMAT_MACROS");
		PrivateDefinitions.Add("_USE_MATH_DEFINES");
		PrivateDefinitions.Add("LZ4_DISABLE_DEPRECATE_WARNINGS");
		PrivateDefinitions.Add("XXH_NAMESPACE=ZSTD_");
		PrivateDefinitions.Add("ZSTD_LEGACY_SUPPORT=0");
		PrivateDefinitions.Add("CMAKE_INTDIR=\"Release\"");
		PrivateDefinitions.Add("ZDICT_DISABLE_DEPRECATE_WARNINGS");
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PrivateDefinitions.Add("_WIN32");
			PrivateDefinitions.Add("WIN32");
			PrivateDefinitions.Add("_WINDOWS"); 
			PrivateDefinitions.Add("_WIN32_WINNT=0x0601");
			PrivateDefinitions.Add("WIN32_LEAN_AND_MEAN");
			PrivateDefinitions.Add("ZSTD_DLL_EXPORT=1");
			PrivateDefinitions.Add("__STDC_LIMIT_MACROS");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
		}
	}
}

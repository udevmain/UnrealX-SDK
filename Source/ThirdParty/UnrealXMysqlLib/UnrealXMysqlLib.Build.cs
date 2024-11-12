// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealXMysqlLib : ModuleRules
{
	public UnrealXMysqlLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange( new string[] {} );
		PrivateIncludePaths.AddRange( new string[] { });
		PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
				}
			);
		PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
					"OpenSSL",
					"DTMysqlZLib",
					"DTMysqlZstd",
				}
			);
		DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
				}
			);
		bEnableUndefinedIdentifierWarnings = false;
		bEnableExceptions = true;
		bUseRTTI = true;
		PrivateDefinitions.Add("NOGDI");
		PrivateDefinitions.Add("NOMINMAX");
		PrivateDefinitions.Add("RAPIDJSON_NO_SIZETYPEDEFINE");
		PrivateDefinitions.Add("RAPIDJSON_SCHEMA_USE_INTERNALREGEX = 0");
		PrivateDefinitions.Add("RAPIDJSON_SCHEMA_USE_STDREGEX = 1");
		PrivateDefinitions.Add("HAVE_CONFIG_H");
		PrivateDefinitions.Add("__STDC_FORMAT_MACROS");
		PrivateDefinitions.Add("_USE_MATH_DEFINES");
		PrivateDefinitions.Add("LZ4_DISABLE_DEPRECATE_WARNINGS");
		PrivateDefinitions.Add("HAVE_TLSv13");
		PrivateDefinitions.Add("CMAKE_INTDIR=\"Release\"");
		PrivateDefinitions.Add("libmysql_EXPORTS");
		PrivateDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
		PrivateDefinitions.Add("DBUG_OFF");
		PrivateDefinitions.Add("NDEBUG");
		PrivateDefinitions.Add("CLIENT_PROTOCOL_TRACING");


		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PrivateDefinitions.Add("_WIN32");
			PrivateDefinitions.Add("WIN32");
			PrivateDefinitions.Add("_WINDOWS");
			PrivateDefinitions.Add("_WIN32_WINNT = 0x0601");
			PrivateDefinitions.Add("WIN32_LEAN_AND_MEAN");
			PrivateDefinitions.Add("SECURITY_WIN32");
			PrivateDefinitions.Add("AUTHENTICATION_WIN");
			PrivateDefinitions.Add("DT_WIN=1");
			PrivateDefinitions.Add("__STDC_LIMIT_MACROS");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateDefinitions.Add("SECURITY_KERNEL");
			PrivateDefinitions.Add("HAVE_ARPA_INET_H");
			PrivateDefinitions.Add("HAVE_POLL_H");
			PrivateDefinitions.Add("HAVE_SYS_UN_H");
			PrivateDefinitions.Add("HAVE_SYS_IOCTL_H");
			PrivateDefinitions.Add("HAVE_SYS_RESOURCE_H");
			PrivateDefinitions.Add("HAVE_SYS_TIME_H");
			PrivateDefinitions.Add("DT_LINUX=1");
		}
	}
}

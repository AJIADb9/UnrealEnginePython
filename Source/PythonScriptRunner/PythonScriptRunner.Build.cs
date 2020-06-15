// Copyright 1998-2018 20Tab S.r.l All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class PythonScriptRunner : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
    public PythonScriptRunner(ReadOnlyTargetRules Target) : base(Target)
#else
    public PythonScriptRunner(TargetInfo Target)
#endif
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        string enableUnityBuild = System.Environment.GetEnvironmentVariable("UEP_ENABLE_UNITY_BUILD");
        bUseUnity = !string.IsNullOrEmpty(enableUnityBuild);

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject", // @todo Mac: for some reason it's needed to link in debug on Mac
                "Engine",
                "Sockets",
                "Networking",
                "UnrealEnginePython",
                // ... add other public dependencies that you statically link with here ...
            }
        );

#if WITH_FORWARDED_MODULE_RULES_CTOR
		if (Target.bBuildEditor)
#else
        if (UEBuildConfiguration.bBuildEditor)
#endif
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd",
                }
            );
        }
    }
}
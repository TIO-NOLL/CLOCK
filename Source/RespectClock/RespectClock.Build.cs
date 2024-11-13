// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RespectClock : ModuleRules
{
	public RespectClock(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
    {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",	// 追加
			"EnhancedInput",
            "NavigationSystem",
            "DeveloperSettings",
            "LevelSequence", 
            "MovieScene"
    });

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] 
            {
                "UnrealEd", 
                "EditorFramework",
                "Json", 
                "JsonUtilities", 
                "Slate", 
                "SlateCore", 
                "AssetTools" ,
                "SourceControl"
            });
        }
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

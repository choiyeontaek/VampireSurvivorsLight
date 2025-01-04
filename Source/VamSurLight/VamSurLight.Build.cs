// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VamSurLight : ModuleRules
{
	public VamSurLight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] {
            "VamSurLight/Common/Default/Public",
            "VamSurLight/Common/GameMode/Public",
            "VamSurLight/Character/Public",
            "VamSurLight/Character/Controller/Public",
            "VamSurLight/Character/DamageType/Public",
            "VamSurLight/Utility/Public"
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

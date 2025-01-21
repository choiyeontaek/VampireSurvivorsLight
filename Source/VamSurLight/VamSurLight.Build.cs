// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VamSurLight : ModuleRules
{
	public VamSurLight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"UMG" /*widget*/
		});

        PublicIncludePaths.AddRange(new string[] {
            "VamSurLight/Common/Default/Public",
            "VamSurLight/Common/GameMode/Public",
            "VamSurLight/DropItem/Experience/Public",
            "VamSurLight/Data/Public",
            "VamSurLight/Character/Public",
            "VamSurLight/Character/Controller/Public",
            "VamSurLight/Character/Weapon/Public",
            "VamSurLight/Character/Synergy/Public",
            "VamSurLight/Character/Level/Public",
            "VamSurLight/Character/Skill/Public",
            "VamSurLight/Character/DamageType/Public",
            "VamSurLight/Character/Widget/Public",
            "VamSurLight/Character/Widget/Skill/Public",
            "VamSurLight/GameUI/Public",
            "VamSurLight/Utility/Public"
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

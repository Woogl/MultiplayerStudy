// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PuzzlePlatforms : ModuleRules
{
	public PuzzlePlatforms(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "OnlineSubsystem" });

        PublicDefinitions.Add("MYDEBUG(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT(x));}");
        PublicDefinitions.Add("MYLOG(x)=UE_LOG(LogTemp, Warning, TEXT(x))");
    }
}

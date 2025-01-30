using UnrealBuildTool;

public class LoadingScreenModule : ModuleRules
{
    public LoadingScreenModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "Public/LoadingScreen.h";
        PCHUsage = PCHUsageMode.UseSharedPCHs; 
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "MoviePlayer", "Slate", "SlateCore" });
    }
}
// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class World : ModuleRules
{
	public World(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Networking", "Sockets" });

        PublicIncludePaths.Add(Path.Combine("Net"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        LoadLua(Target);
	}

    private bool LoadLua(ReadOnlyTargetRules TargetRules)
    {
        bool isLibSupported = false;

        // Check if we are on Windows
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibSupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32"; // This string is either "x64" or "x86" so we can append it on the lib filename
            string LibrariesPath = Path.Combine(ThirdPartyPath, "Lua", "Libraries");
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, "Release", "Lua.lib"));

            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Lua", "Includes"));
        }

        if (isLibSupported)
        {
            Definitions.Add(string.Format("WITH_LUA_BINDING=1"));
        }
        return isLibSupported;
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }
}

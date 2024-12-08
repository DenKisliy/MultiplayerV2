// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class Multiplayer : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}
	
	private string MultiplayerPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../Multiplayer/")); }
	}

	public Multiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SlateCore", "Slate", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SQLiteCore", "SQLiteSupport" });

		PublicIncludePaths.Add("../DataBaseConnectorC64/include");

		bool LibSupportedBool = false;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			LibSupportedBool = true;
			PublicAdditionalLibraries.Add(Path.Combine(MultiplayerPath, "../DataBaseConnectorC64/lib", "mariadbclient.lib"));
		}

		PublicDefinitions.Add(string.Format("WITH_JUCE_BINDING={0}", LibSupportedBool ? 1 : 0));
	}
}

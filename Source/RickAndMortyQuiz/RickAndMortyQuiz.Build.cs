// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// At top of .Build.cs file
using System.IO;


public class RickAndMortyQuiz : ModuleRules
{
	public RickAndMortyQuiz(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../Plugins/RickAndMortyWidget/Source/RickAndMortyWidget/Public"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../Plugins/RickAndMortyWidget/Source/RickAndMortyWidget/Private"));

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RickAndMortyWidget" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

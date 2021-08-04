// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// At top of .Build.cs file
using System.IO;

public class RickAndMortyWidget : ModuleRules
{
	public RickAndMortyWidget(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../../../Source/RickAndMortyQuiz"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../../../Source/RickAndMortyQuiz"));

		PublicIncludePaths.AddRange(
			new string[] {

			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Http", "Json", "JsonUtilities", "UMG"
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

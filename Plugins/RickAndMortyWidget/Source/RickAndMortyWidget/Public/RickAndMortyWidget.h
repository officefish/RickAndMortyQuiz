// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRickAndMortyWidgetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// Declare special Log Category for Widget
DECLARE_LOG_CATEGORY_EXTERN(WBP_LogRickAndMorty, Log, All);
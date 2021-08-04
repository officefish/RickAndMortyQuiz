// Copyright Epic Games, Inc. All Rights Reserved.

#include "RickAndMortyWidget.h"

#define LOCTEXT_NAMESPACE "FRickAndMortyWidgetModule"

// Define special Log Category for Widget
DEFINE_LOG_CATEGORY(WBP_LogRickAndMorty);

void FRickAndMortyWidgetModule::StartupModule()
{
	UE_LOG(WBP_LogRickAndMorty, Log, TEXT("RickAndMortyWidget::StartupModule"));
}

void FRickAndMortyWidgetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRickAndMortyWidgetModule, RickAndMortyWidget)
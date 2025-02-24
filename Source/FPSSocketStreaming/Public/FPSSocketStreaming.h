// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define DO_TRACKING 1

#define COM_PORT 3
#define BAUD_RATE 115200

#define FILE_DESTINATION "/FPSSocketStreaming/Output/Results.csv"

#if DO_TRACKING

class FFPSSocketStreamingModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
#endif
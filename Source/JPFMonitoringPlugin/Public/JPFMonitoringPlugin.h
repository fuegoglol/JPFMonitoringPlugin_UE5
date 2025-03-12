// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Do the game quit himself after a delay
#define DO_CLOSE_GAME_AFTER_DELAY true
#define BENCH_DELAY_IN_SECONDS 120.f
// Number of the com port to read data from
#define COM_PORT 3
// Read speed on the serial port
#define BAUD_RATE 9600
// Path of the outputed csv file
#define FILE_DESTINATION "/JPFMonitoringPlugin/Output/"
#define BASE_FILENAME "Results"

class FJPFMonitoringPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};

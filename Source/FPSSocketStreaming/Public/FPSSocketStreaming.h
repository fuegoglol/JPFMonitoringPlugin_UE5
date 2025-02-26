// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// If true, the outputed csv will contain the time of the measure
#define TRACK_TIME 1
// Number of the com port to read data from
#define COM_PORT 7
// Read speed on the serial port
#define BAUD_RATE 9600
// Path of the outputed csv file
#define FILE_DESTINATION "/FPSSocketStreaming/Output/"
#define BASE_FILENAME "Results"

class FFPSSocketStreamingModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};

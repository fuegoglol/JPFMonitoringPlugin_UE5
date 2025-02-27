// Fill out your copyright notice in the Description page of Project Settings.

#include "DataStreamingSubsystem.h"

#include "JPFMonitoringPlugin.h"
#include "SerialCom.h"


void UDataStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);

	// COM PORT OPENING
	bool bIsPortOpened = false;
	SerialCom = USerialCom::OpenComPortWithFlowControl(bIsPortOpened, COM_PORT, BAUD_RATE);
	if(!bIsPortOpened)
	{
		UE_LOG(LogTemp,Error,TEXT("Can't open port COM%d"),COM_PORT);
		return;
	}

	SerialCom->Flush();

	// RESULTS FILE OPENING

	FinalFileDestination = FPlatformProcess::UserDir()+FString(FILE_DESTINATION);

}

void UDataStreamingSubsystem::Deinitialize()
{
	SerialCom->Close();

	//Write results in file

	FString Results;
	Results.Append("Power");
	for (const auto Power : PowerArray)
	{
		Results.Append(FString(", ")+FString::SanitizeFloat(Power,2));
	}
	Results.Append("\n");
	Results.Append("FPS");
	for (const auto FPS : FPSArray)
	{
		Results.Append(FString(", ")+FString::SanitizeFloat(FPS,0));
	}
	Results.Append("\n");
	
	FFileHelper::SaveStringToFile(Results,*FinalFileDestination);
	
	Super::Deinitialize();
}

void UDataStreamingSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bool bDidRead = false;
	const FString ReadResults = SerialCom->Readln(bDidRead);
	if(bDidRead)
	{
		PowerArray.Add(FCString::Atof(*ReadResults));
		FPSArray.Add(1000/DeltaTime);
	}
}

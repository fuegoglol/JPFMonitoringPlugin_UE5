// Fill out your copyright notice in the Description page of Project Settings.

#include "DataStreamingSubsystem.h"

#include "JPFMonitoringPlugin.h"
#include "SerialCom.h"


void UDataStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);

	Measures.Empty();

	// COM PORT OPENING
	bIsPortOpened = false;
	SerialCom = USerialCom::OpenComPortWithFlowControl(bIsPortOpened, COM_PORT, BAUD_RATE);
	if(!bIsPortOpened)
	{
		UE_LOG(LogTemp,Error,TEXT("Can't open port COM%d"),COM_PORT);
		return;
	}

	SerialCom->Flush();

	// RESULTS FILE OPENING

	FinalFileDestination = FString::Printf(TEXT("%s%hs%hs_%s.csv"),
		FPlatformProcess::UserDir(),
		FILE_DESTINATION,
		BASE_FILENAME,
		*GetWorld()->GetMapName());

}

void UDataStreamingSubsystem::Deinitialize()
{
	if(!bIsPortOpened || !SerialCom)
		return;

	SerialCom->Close();


	//Write results in file

	FString Results;
	

	UE_LOG(LogTemp, Display,TEXT("Measures length : %i"),Measures.Num());
	Results.Append(FMeasure::DisplayHeaderRow());
	for (auto Measure: Measures)
	{
		Results.Append(Measure.DisplayRow());
	}

	
	FFileHelper::SaveStringToFile(Results,*FinalFileDestination);
	
	Super::Deinitialize();
}

void UDataStreamingSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time+=DeltaTime;
	
	if(!bIsPortOpened || !SerialCom)
		return;
	
	bool bDidRead = false;
	const FString ReadResults = SerialCom->Readln(bDidRead);
	if(bDidRead)
	{
		/*PowerArray.Add(FCString::Atof(*ReadResults));
		FPSArray.Add(1000/DeltaTime);*/
		FMeasure M;
		M.FPS = 1.0f/DeltaTime;
		M.Power = FCString::Atof(*ReadResults);
		M.Time = Time;

		Measures.Add(M);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "DataStreamingSubsystem.h"
#include "Misc/DateTime.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "JPFMonitoringPlugin.h"
#include "SerialCom.h"
#include "Kismet/KismetSystemLibrary.h"


void UDataStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UDataStreamingSubsystem::Tick));

	Super::Initialize(Collection);

	Measures.Empty();

	// COM PORT OPENING
	bIsPortOpened = false;
	SerialCom = USerialCom::OpenComPortWithFlowControl(bIsPortOpened, COM_PORT, BAUD_RATE);

	if(!bIsPortOpened)
	{

		UE_LOG(LogTemp,Warning,TEXT("Can't open port COM%d"),COM_PORT);
#if DO_CLOSE_GAME_AFTER_DELAY
		UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,true);
#endif
		return;
	}

	SerialCom->Flush();

	// RESULTS FILE OPENING

	
	
	FinalFileDestination = FString::Printf(TEXT("%s%hs%s/%hs_%s_%lli.csv"),
		FPlatformProcess::UserDir(),
		FILE_DESTINATION,
		FPlatformProcess::ExecutableName(),
		BASE_FILENAME,
		FPlatformProcess::ExecutableName(),
		FDateTime::Now().GetTicks());

}

void UDataStreamingSubsystem::Deinitialize()
{
	FString Results;
	
	if(!bIsPortOpened || !SerialCom)
	{
		Results = "Error reading values";
		
		FFileHelper::SaveStringToFile(Results,*FinalFileDestination);
		return;
	}
		

	SerialCom->Close();


	//Write results in file


	UE_LOG(LogTemp, Display,TEXT("Measures length : %i"),Measures.Num());
	Results.Append(FMeasure::DisplayHeaderRow());
	for (auto Measure: Measures)
	{
		Results.Append(Measure.DisplayRow());
	}

	
	FFileHelper::SaveStringToFile(Results,*FinalFileDestination);
	
	Super::Deinitialize();
}

bool UDataStreamingSubsystem::Tick(float DeltaTime)
{

	Time+=DeltaTime;
	
	if(!bIsPortOpened || !SerialCom)
		return true;
	
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

#if DO_CLOSE_GAME_AFTER_DELAY
	if(Time >= BENCH_DELAY_IN_SECONDS)
		
#if UE_SERVER
		FGenericPlatformMisc::RequestExit(false,TEXT("Killing process after desired bench delay"));
#else
		UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Type::Quit,true);
#endif
		
#endif
	
	
	return true;
}

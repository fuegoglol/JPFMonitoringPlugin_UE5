// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSSocketStreaming.h"
#include "DataStreamingSubsystem.generated.h"

class USerialCom;

USTRUCT()
struct FMeasure
{
	GENERATED_BODY()
	
	float Time = 0;
	float FPS = 0.f;
	float Power = 0.f;

	FString DisplayRow() const{
		return FString::Printf(TEXT("%5.4f, %5.2f, %d\n"), Time, Power, static_cast<int>(FMath::RoundHalfToEven(FPS)));
	};

	static FString DisplayHeaderRow() 
	{
		return FString("Time, Power, FPS\n");
	}
};


/**
 * 
 */
UCLASS()
class UDataStreamingSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

protected:

	TObjectPtr<USerialCom> SerialCom;

	//TArray<float> PowerArray;
	//TArray<float> FPSArray;

	UPROPERTY()
	TArray<FMeasure> Measures;

	float Time = 0;


	FString FinalFileDestination;
	bool bIsPortOpened;

private:

	FTSTicker::FDelegateHandle TickDelegateHandle;
};

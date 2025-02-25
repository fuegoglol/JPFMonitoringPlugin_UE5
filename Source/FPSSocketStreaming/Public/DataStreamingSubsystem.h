// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStreamingSubsystem.generated.h"

class USerialCom;


struct Measure
{
	int64 Time;
	float FPS;
	float Power;

	FString DisplayRow() const{
		return Printf(TEXT("%i, $i, $f\n"), Time, FString::SanitizeFloat(Power, 2), FString::SanitizeFloat(FPS, 0));
	};

	static FString DisplayHeaderRow() 
	{
		return FString("Time, Power, FPS\n");
	}
}


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
        TArray<Measure> Measures;


	FString FinalFileDestination;

private:

	FTSTicker::FDelegateHandle TickDelegateHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStreamingSubsystem.generated.h"

class USerialCom;
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

	TArray<float> PowerArray;
	TArray<float> FPSArray;

	FString FinalFileDestination;

private:

	FTSTicker::FDelegateHandle TickDelegateHandle;
};

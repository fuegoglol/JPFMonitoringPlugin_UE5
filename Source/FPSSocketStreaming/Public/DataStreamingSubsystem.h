// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/TcpListener.h"
#include "Common/TcpSocketBuilder.h"
#include "DataStreamingSubsystem.generated.h"


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7787
#define BUFFER_SIZE 4096

/**
 * 
 */
UCLASS()
class UDataStreamingSubsystem : public UGameInstance
{
	GENERATED_BODY()

public:
	
	virtual void Init() override;

	virtual void Shutdown() override;
	
	bool Tick(float DeltaTime);

protected:

	bool ClientConnected(FSocket* Socket, const FIPv4Endpoint& Endpoint);

	void SendDataToClient(float Framerate);
	

private:
	FSocket* ClientSocket = nullptr;
	
	TUniquePtr<FTcpListener> ServerListner;
	FTSTicker::FDelegateHandle TickDelegateHandle;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/TcpListener.h"
#include "Common/TcpSocketBuilder.h"
#include "Modules/ModuleManager.h"

#define DO_TRACKING 1

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7787
#define BUFFER_SIZE 4096

#if DO_TRACKING

class FFPSSocketStreamingModule : public IModuleInterface, FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:

	bool ClientConnected(FSocket* Socket, const FIPv4Endpoint& Endpoint);

	void SendDataToClient(float Framerate);

public:
	virtual void Tick(float DeltaTime) override;

private:

	FSocket* ClientSocket = nullptr;
	
	TUniquePtr<FTcpListener> ServerListner;
};
#endif
// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSSocketStreaming.h"

#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#if DO_TRACKING
#define LOCTEXT_NAMESPACE "FFPSSocketStreamingModule"


void FFPSSocketStreamingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FIPv4Address IPAddress;
	FIPv4Address::Parse(FString(SERVER_IP),IPAddress);
	FIPv4Endpoint Endpoint(IPAddress,SERVER_PORT);

	ServerListner = MakeUnique<FTcpListener>(Endpoint);

	ServerListner->OnConnectionAccepted().BindUObject(this,&FFPSSocketStreamingModule::ClientConnected);

	ServerListner->Run();

}

bool FFPSSocketStreamingModule::ClientConnected(FSocket* Socket,const FIPv4Endpoint& Endpoint)
{

	ClientSocket = Socket;
	return true;
}

void FFPSSocketStreamingModule::SendDataToClient(float Framerate)
{
	TArray<uint8> Bytes;
	FMemoryWriter MemoryWriter(Bytes);

	MemoryWriter << Framerate;

	int32 BytesSent = 0;
	if(!ClientSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to stream framerate"));
	}
}

void FFPSSocketStreamingModule::Tick(float DeltaTime)
{
	if(ClientSocket)
		SendDataToClient(1.0f/DeltaTime);
}

void FFPSSocketStreamingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	ServerListner->Exit();

	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFPSSocketStreamingModule, FPSSocketStreaming)
#endif

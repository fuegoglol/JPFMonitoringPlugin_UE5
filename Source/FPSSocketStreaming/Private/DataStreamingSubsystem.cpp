// Fill out your copyright notice in the Description page of Project Settings.

#include "DataStreamingSubsystem.h"

#include "Common/TcpSocketBuilder.h"
#include "Sockets.h"


void UDataStreamingSubsystem::Init()
{
	// Register delegate for ticker callback
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this,&UDataStreamingSubsystem::Tick));

	Super::Init();
	
	FIPv4Address IPAddress;
	FIPv4Address::Parse(FString(SERVER_IP),IPAddress);
	FIPv4Endpoint Endpoint(IPAddress,SERVER_PORT);

	ServerListner = MakeUnique<FTcpListener>(Endpoint);

	ServerListner->OnConnectionAccepted().BindUObject(this,&UDataStreamingSubsystem::ClientConnected);

	ServerListner->Run();

}

void UDataStreamingSubsystem::Shutdown()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Super::Shutdown();
}

bool UDataStreamingSubsystem::Tick(float DeltaTime)
{
	if(ClientSocket)
		SendDataToClient(1.0f/DeltaTime);

	return true;
}


bool UDataStreamingSubsystem::ClientConnected(FSocket* Socket,const FIPv4Endpoint& Endpoint)
{

	ClientSocket = Socket;
	return true;
}

void UDataStreamingSubsystem::SendDataToClient(float Framerate)
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
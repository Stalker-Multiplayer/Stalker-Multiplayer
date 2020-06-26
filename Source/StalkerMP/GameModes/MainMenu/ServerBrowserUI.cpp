// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerBrowserUI.h"

#include "StalkerMP/DataClasses/ServerData.h"
#include "StalkerMP/GameModes/MainMenu/MainMenuPlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"


void UServerBrowserUI::NativeConstruct()
{
	Super::NativeConstruct();

	UStalkerMPGameInstance::FOnSessionsFoundDelegate OnSessionsFoundDelegate = UStalkerMPGameInstance::FOnSessionsFoundDelegate::CreateUObject(this, &UServerBrowserUI::OnServersFound);
	OnSessionsFoundDelegateHandle = GetGameInstance<UStalkerMPGameInstance>()->AddOnSessionsFoundDelegate_Handle(OnSessionsFoundDelegate);

	UStalkerMPGameInstance::FOnSessionFindCanceledDelegate OnSessionsFindCanceledDelegate = UStalkerMPGameInstance::FOnSessionFindCanceledDelegate::CreateUObject(this, &UServerBrowserUI::OnFindServersCanceled);
	OnSessionsFindCanceledDelegateHandle = GetGameInstance<UStalkerMPGameInstance>()->AddOnSessionFindCanceledDelegate_Handle(OnSessionsFindCanceledDelegate);

	UStalkerMPGameInstance::FOnSessionJoinCompleteDelegate OnSessionJoinCompleteDelegate = UStalkerMPGameInstance::FOnSessionJoinCompleteDelegate::CreateLambda([this](FString TravelURL, const EOnSessionJoinResult ResultType)
	{
		switch (ResultType)
		{
		case EOnSessionJoinResult::Success:
		case EOnSessionJoinResult::AlreadyInSession:
			GetOwningPlayer<AMainMenuPlayerController>()->ShowLoadingScreen();
			GetOwningPlayer<AMainMenuPlayerController>()->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			break;
		case EOnSessionJoinResult::SessionIsFull:
			ShowErrorMessage(true, FText::FromString("Session is full"));
			break;
		case EOnSessionJoinResult::SessionDoesNotExist:
			ShowErrorMessage(true, FText::FromString("Session does not exist"));
			break;
		case EOnSessionJoinResult::CouldNotRetrieveAddress:
			ShowErrorMessage(true, FText::FromString("Could not retrieve address"));
			break;
		case EOnSessionJoinResult::UnknownError:
			ShowErrorMessage(true, FText::FromString("Unknown error"));
			break;
		}
	});
	OnSessionJoinCompleteDelegateHandle = GetGameInstance<UStalkerMPGameInstance>()->AddOnSessionJoinCompleteDelegate_Handle(OnSessionJoinCompleteDelegate);
}

void UServerBrowserUI::NativeDestruct()
{
	Super::NativeDestruct();

	GetGameInstance<UStalkerMPGameInstance>()->ClearOnSessionsFoundDelegate_Handle(OnSessionsFoundDelegateHandle);
	GetGameInstance<UStalkerMPGameInstance>()->ClearOnSessionFindCanceledDelegate_Handle(OnSessionsFindCanceledDelegateHandle);
	GetGameInstance<UStalkerMPGameInstance>()->ClearOnSessionJoinCompleteDelegate_Handle(OnSessionJoinCompleteDelegateHandle);
}

void UServerBrowserUI::RefreshList()
{
	ShowProgressMessage(true, FText::FromString("Searching..."));
	GetGameInstance<UStalkerMPGameInstance>()->FindServers();
}

void UServerBrowserUI::CancelCurrentProgress()
{
	GetGameInstance<UStalkerMPGameInstance>()->CancelFindServers();
}

void UServerBrowserUI::OnServersFound(TArray<UServerData*> Servers)
{
	ShowProgressMessage(false, FText::FromString(""));
	SetServersList(Servers);

	if (Servers.Num() <= 0)
	{
		ShowErrorMessage(true, FText::FromString("No servers found"));
	}
}

void UServerBrowserUI::OnFindServersCanceled(bool Successful)
{
	ShowProgressMessage(false, FText::FromString(""));
}

void UServerBrowserUI::CloseServerBrowser()
{
	GetOwningPlayer<AMainMenuPlayerController>()->ShowMainMenu();
}

void UServerBrowserUI::CloseErrorMessage()
{
	ShowErrorMessage(false, FText::FromString(""));
}

void UServerBrowserUI::ConnectToServer(FName ServerAddress)
{
	GetOwningPlayer<AMainMenuPlayerController>()->ConnectToServer(ServerAddress);
}

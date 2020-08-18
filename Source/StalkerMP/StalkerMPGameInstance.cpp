// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerMPGameInstance.h"
#include "DataClasses/ServerData.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
#include "Public/OnlineSubsystem.h"
#include "Public/OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Engine/LocalPlayer.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


const FString UStalkerMPGameInstance::SERVER_NAME_SETTING_KEY = "ServerName";
const FString UStalkerMPGameInstance::DEFAULT_SERVER_NAME = "A StalkerMP server";
const FString UStalkerMPGameInstance::MAX_PLAYERS_SETTING_KEY = "MaxPlayers";
const int UStalkerMPGameInstance::DEFAULT_MAX_PLAYERS = 20;

FOnFloatSettingValueUpdatedDelegate UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate = FOnFloatSettingValueUpdatedDelegate();
FOnIntSettingValueUpdatedDelegate UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate = FOnIntSettingValueUpdatedDelegate();
FOnStringSettingValueUpdatedDelegate UStalkerMPGameInstance::OnStringSettingValueUpdatedDelegate = FOnStringSettingValueUpdatedDelegate();

void UStalkerMPGameInstance::Init()
{
	//Super::Init();

	LoadAllSettings();

	if (!IsDedicatedServerInstance())
	{
		InitOnlineSubsystem();
	}
}

void UStalkerMPGameInstance::Shutdown()
{
	//Super::Shutdown();

	DestroyOnlineSubsystem();
}




// ######################################################################### User Settings #########################################################################

void UStalkerMPGameInstance::LoadAllSettings()
{
	LoadSettings(ESettingsType::General);
	LoadSettings(ESettingsType::Video);
	LoadSettings(ESettingsType::Sound);
	LoadSettings(ESettingsType::Controls);

	if (IsDedicatedServerInstance())
	{
		ServerSettingsFileName = ParseLaunchOption("ServerConfig", ServerSettingsFileName);
		UE_LOG(LogTemp, Warning, TEXT("Using config file '%s'"), *ServerSettingsFileName);
		LoadSettings(ESettingsType::Server);
	}
}

FString UStalkerMPGameInstance::ParseLaunchOption(FString Key, FString Fallback)
{
	TArray<FString> OptionsArray;
	FString CommandLine = UKismetSystemLibrary::GetCommandLine();
	CommandLine.ParseIntoArray(OptionsArray, TEXT(" "), true);

	for (FString Option : OptionsArray)
	{
		if (Option.StartsWith("-" + Key)) {
			FString _;
			FString Value;
			Option.Split("=", &_, &Value);

			return Value;
		}
	}

	return Fallback;
}

void UStalkerMPGameInstance::LoadSettings(ESettingsType SettingsType)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		TMap<FString, FString>* MapToSaveTo;
		FString FileName;
		switch (SettingsType)
		{
		case ESettingsType::General:
			MapToSaveTo = &Map_GeneralSettings;
			FileName = GeneralSettingsFileName;
			break;
		case ESettingsType::Video:
			MapToSaveTo = &Map_VideoSettings;
			FileName = VideoSettingsFileName;
			break;
		case ESettingsType::Sound:
			MapToSaveTo = &Map_SoundSettings;
			FileName = SoundSettingsFileName;
			break;
		case ESettingsType::Controls:
			MapToSaveTo = &Map_ControlsSettings;
			FileName = ControlsSettingsFileName;
			break;
		case ESettingsType::Server:
			MapToSaveTo = &Map_ServerSettings;
			FileName = ServerSettingsFileName;
			break;
		default:
			TMap<FString, FString> TempMap;
			MapToSaveTo = &TempMap;
			FileName = "";
		}

		FString AbsoluteFilePath = SaveDirectory + "/" + *FileName;

		MapToSaveTo->Empty();
		TArray<FString> LoadedSettings;
		FFileHelper::LoadFileToStringArray(LoadedSettings, *AbsoluteFilePath, FFileHelper::EHashOptions::None);

		for (FString Setting : LoadedSettings)
		{
			FString Key;
			FString Value;

			Setting.TrimStartAndEndInline();

			if (Setting.Contains("#"))
			{
				Setting.Split("#", &Setting, &Value);
			}
			Setting.Split("=", &Key, &Value);

			Key.TrimStartAndEndInline();
			Value.TrimStartAndEndInline();

			PutStringSettingValue(SettingsType, Key, Value);
		}
	}
}

void UStalkerMPGameInstance::SaveSettings(ESettingsType SettingsType)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		TMap<FString, FString>* MapToSave;
		FString FileName;
		switch (SettingsType)
		{
		case ESettingsType::General:
			MapToSave = &Map_GeneralSettings;
			FileName = GeneralSettingsFileName;
			break;
		case ESettingsType::Video:
			MapToSave = &Map_VideoSettings;
			FileName = VideoSettingsFileName;
			break;
		case ESettingsType::Sound:
			MapToSave = &Map_SoundSettings;
			FileName = SoundSettingsFileName;
			break;
		case ESettingsType::Controls:
			MapToSave = &Map_ControlsSettings;
			FileName = ControlsSettingsFileName;
			break;
		case ESettingsType::Server:
			MapToSave = &Map_ServerSettings;
			FileName = ServerSettingsFileName;
			break;
		default:
			TMap<FString, FString> TempMap;
			MapToSave = &TempMap;
			FileName = "";
		}

		FString AbsoluteFilePath = SaveDirectory + "/" + *FileName;

		TArray<FString> LinesToSave;
		for (const TPair<FString, FString>& pair : *MapToSave)
		{
			if (!pair.Key.IsEmpty() && !pair.Value.IsEmpty())
			{
				LinesToSave.Add(pair.Key + "=" + pair.Value);
			}
		}

		FFileHelper::SaveStringArrayToFile(LinesToSave, *AbsoluteFilePath);
	}
}

float UStalkerMPGameInstance::GetFloatSettingValue(ESettingsType SettingsType, FString Key, float Fallback)
{
	FString Value = GetStringSettingValue(SettingsType, Key, "");
	if (Value.IsNumeric())
	{
		return FCString::Atof(*Value);
	}
	else
	{
		return Fallback;
	}
}

int UStalkerMPGameInstance::GetIntSettingValue(ESettingsType SettingsType, FString Key, int Fallback)
{
	FString Value = GetStringSettingValue(SettingsType, Key, "");
	if (Value.IsNumeric())
	{
		return FCString::Atoi(*Value);
	}
	else
	{
		return Fallback;
	}
}

FString UStalkerMPGameInstance::GetStringSettingValue(ESettingsType SettingsType, FString Key, FString Fallback)
{
	TMap<FString, FString>* MapToGetValueFrom;
	switch (SettingsType)
	{
	case ESettingsType::General:
		MapToGetValueFrom = &Map_GeneralSettings;
		break;
	case ESettingsType::Video:
		MapToGetValueFrom = &Map_VideoSettings;
		break;
	case ESettingsType::Sound:
		MapToGetValueFrom = &Map_SoundSettings;
		break;
	case ESettingsType::Controls:
		MapToGetValueFrom = &Map_ControlsSettings;
		break;
	case ESettingsType::Server:
		MapToGetValueFrom = &Map_ServerSettings;
		break;
	default:
		TMap<FString, FString> TempMap;
		MapToGetValueFrom = &TempMap;
	}

	if (MapToGetValueFrom->Contains(Key))
	{
		return (*MapToGetValueFrom)[Key];
	}
	else
	{
		return Fallback;
	}
}

void UStalkerMPGameInstance::RemoveSetting(ESettingsType SettingsType, FString Key)
{
	TMap<FString, FString>* MapToRemoveValueFrom;
	switch (SettingsType)
	{
	case ESettingsType::General:
		MapToRemoveValueFrom = &Map_GeneralSettings;
		break;
	case ESettingsType::Video:
		MapToRemoveValueFrom = &Map_VideoSettings;
		break;
	case ESettingsType::Sound:
		MapToRemoveValueFrom = &Map_SoundSettings;
		break;
	case ESettingsType::Controls:
		MapToRemoveValueFrom = &Map_ControlsSettings;
		break;
	case ESettingsType::Server:
		MapToRemoveValueFrom = &Map_ServerSettings;
		break;
	default:
		TMap<FString, FString> TempMap;
		MapToRemoveValueFrom = &TempMap;
	}

	MapToRemoveValueFrom->Remove(Key);
}

void UStalkerMPGameInstance::PutFloatSettingValue(ESettingsType SettingsType, FString Key, float Value)
{
	PutSettingValue(SettingsType, Key, FString::SanitizeFloat(Value));
	OnFloatSettingValueUpdatedDelegate.Broadcast(Key, Value);
}

void UStalkerMPGameInstance::PutIntSettingValue(ESettingsType SettingsType, FString Key, int Value)
{
	PutSettingValue(SettingsType, Key, FString::FromInt(Value));
	OnIntSettingValueUpdatedDelegate.Broadcast(Key, Value);
}

void UStalkerMPGameInstance::PutStringSettingValue(ESettingsType SettingsType, FString Key, FString Value)
{
	PutSettingValue(SettingsType, Key, Value);
	OnStringSettingValueUpdatedDelegate.Broadcast(Key, Value);
}

void UStalkerMPGameInstance::PutSettingValue(ESettingsType SettingsType, FString Key, FString Value)
{
	TMap<FString, FString>* MapToPutValueTo;
	switch (SettingsType)
	{
	case ESettingsType::General:
		MapToPutValueTo = &Map_GeneralSettings;
		break;
	case ESettingsType::Video:
		MapToPutValueTo = &Map_VideoSettings;
		break;
	case ESettingsType::Sound:
		MapToPutValueTo = &Map_SoundSettings;
		break;
	case ESettingsType::Controls:
		MapToPutValueTo = &Map_ControlsSettings;
		break;
	case ESettingsType::Server:
		MapToPutValueTo = &Map_ServerSettings;
		break;
	default:
		TMap<FString, FString> TempMap;
		MapToPutValueTo = &TempMap;
	}

	MapToPutValueTo->Add(Key, Value);
}




// ######################################################################### Online Subsystem #########################################################################

FString UStalkerMPGameInstance::GetGameVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

void UStalkerMPGameInstance::InitOnlineSubsystem()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UStalkerMPGameInstance::OnFindSessionsComplete);
		OnFindSessionsCompleteDelegateHandle = SessionInt->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

		FOnCancelFindSessionsCompleteDelegate OnCancelFindSessionsCompleteDelegate = FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &UStalkerMPGameInstance::OnCancelFindSessionsComplete);
		OnCancelFindSessionsCompleteDelegateHandle = SessionInt->AddOnCancelFindSessionsCompleteDelegate_Handle(OnCancelFindSessionsCompleteDelegate);

		FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateLambda([this](FName SessionName, const EOnJoinSessionCompleteResult::Type ResultType)
		{
			switch (ResultType)
			{
			case EOnJoinSessionCompleteResult::Success:
			case EOnJoinSessionCompleteResult::AlreadyInSession:
			{
				IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

				if (OnlineSub)
				{
					IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();
					ConnectedSessionName = SessionName.ToString();
					FString TravelURL;
					APlayerController* PlayerController = GetLocalPlayerByIndex(0)->GetPlayerController(GetWorld());
					if (PlayerController && SessionInt->GetResolvedConnectString(SessionName, TravelURL))
					{
						TriggerOnSessionJoinCompleteDelegates(TravelURL, EOnSessionJoinResult::Success);
						break;
					}
				}
				TriggerOnSessionJoinCompleteDelegates("", EOnSessionJoinResult::UnknownError);
			}
			break;
			case EOnJoinSessionCompleteResult::SessionIsFull:
				TriggerOnSessionJoinCompleteDelegates("", EOnSessionJoinResult::SessionIsFull);
				break;
			case EOnJoinSessionCompleteResult::SessionDoesNotExist:
				TriggerOnSessionJoinCompleteDelegates("", EOnSessionJoinResult::SessionDoesNotExist);
				break;
			case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
				TriggerOnSessionJoinCompleteDelegates("", EOnSessionJoinResult::CouldNotRetrieveAddress);
				break;
			case EOnJoinSessionCompleteResult::UnknownError:
				TriggerOnSessionJoinCompleteDelegates("", EOnSessionJoinResult::UnknownError);
				break;
			}
		});
		OnJoinSessionCompleteDelegateHandle = SessionInt->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

		OnNetworkFailureDelegateHandle = GetEngine()->OnNetworkFailure().AddUObject(this, &UStalkerMPGameInstance::OnNetworkFailure);
	}
}

void UStalkerMPGameInstance::DestroyOnlineSubsystem()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		SessionInt->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
		SessionInt->ClearOnCancelFindSessionsCompleteDelegate_Handle(OnCancelFindSessionsCompleteDelegateHandle);
		SessionInt->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		GetEngine()->OnNetworkFailure().Remove(OnNetworkFailureDelegateHandle);
	}
}

void UStalkerMPGameInstance::CreateSession()
{
	int MaxPlayers = GetIntSettingValue(ESettingsType::Server, MAX_PLAYERS_SETTING_KEY, DEFAULT_MAX_PLAYERS);
	FString ServerName = GetStringSettingValue(ESettingsType::Server, SERVER_NAME_SETTING_KEY, DEFAULT_SERVER_NAME);

	FOnlineSessionSettings Settings;
	Settings.bIsDedicated = true;
	Settings.NumPublicConnections = MaxPlayers;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = false;
	Settings.bUsesPresence = false;
	Settings.bAllowJoinViaPresence = false;

	int RandomInt = UKismetMathLibrary::RandomInteger(8999999) + 1000000;
	TheSessionName = "Session" + FString::FromInt(RandomInt);
	Settings.Set<FString>(TEXT("SessionName"), TheSessionName, EOnlineDataAdvertisementType::Type::ViaOnlineService);
	Settings.Set<FString>(TEXT("ServerName"), ServerName, EOnlineDataAdvertisementType::Type::ViaOnlineService);
	Settings.Set<FString>(TEXT("GameMode"), CurrentGameMode, EOnlineDataAdvertisementType::Type::ViaOnlineService);
	Settings.Set<FString>(TEXT("MapName"), CurrentMapName, EOnlineDataAdvertisementType::Type::ViaOnlineService);
	Settings.Set<FString>(TEXT("GameVersion"), GetGameVersion(), EOnlineDataAdvertisementType::Type::ViaOnlineService);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();
		SessionInt->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		SessionInt->CreateSession(0, FName(*TheSessionName), Settings);
	}
}

void UStalkerMPGameInstance::UpdateSession(FString GameMode, FString MapName)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		CurrentGameMode = GameMode;
		CurrentMapName = MapName;
		if (!TheSessionName.IsEmpty())
		{
			IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

			FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UStalkerMPGameInstance::OnDestroySessionComplete);
			OnDestroySessionCompleteDelegateHandle = SessionInt->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			SessionInt->DestroySession(FName(*TheSessionName));
		}
		else
		{
			CreateSession();
		}
	}
}

void UStalkerMPGameInstance::OnDestroySessionComplete(FName SessionName, bool Successful)
{
	CreateSession();
}

void UStalkerMPGameInstance::FindServers()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
		OnlineSessionSearch->bIsLanQuery = false;
		OnlineSessionSearch->MaxSearchResults = 1000;

		SessionInt->FindSessions(0, OnlineSessionSearch.ToSharedRef());
	}
}

void UStalkerMPGameInstance::OnFindSessionsComplete(bool Successful)
{
	if (Successful)
	{
		TArray<UServerData*> ServerDatas;

		if (OnlineSessionSearch)
		{
			for (FOnlineSessionSearchResult SearchResult : OnlineSessionSearch.Get()->SearchResults)
			{
				FString ServerName;
				SearchResult.Session.SessionSettings.Get<FString>(TEXT("ServerName"), ServerName);
				FString GameMode;
				SearchResult.Session.SessionSettings.Get<FString>(TEXT("GameMode"), GameMode);
				FString MapName;
				SearchResult.Session.SessionSettings.Get<FString>(TEXT("MapName"), MapName);
				FString GameVersion;
				SearchResult.Session.SessionSettings.Get<FString>(TEXT("GameVersion"), GameVersion);

				if (!ServerName.IsEmpty() && !GameMode.IsEmpty() && !MapName.IsEmpty() && !GameVersion.IsEmpty())
				{
					UServerData* ServerData = NewObject<UServerData>();
					ServerData->SetSessionId(SearchResult.GetSessionIdStr());
					ServerData->SetServerName(ServerName);
					ServerData->SetPing(SearchResult.PingInMs);
					ServerData->SetPlayersCount(SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections);
					ServerData->SetMaxPlayersCount(SearchResult.Session.SessionSettings.NumPublicConnections);
					ServerData->SetGameMode(GameMode);
					ServerData->SetMapName(MapName);
					ServerData->SetGameVersion(GameVersion);

					ServerDatas.Add(ServerData);
				}
			}
		}

		TriggerOnSessionsFoundDelegates(ServerDatas);
	}
	else
	{
		TArray<UServerData*> ServerDatas;

		TriggerOnSessionsFoundDelegates(ServerDatas);
	}
}

void UStalkerMPGameInstance::OnCancelFindSessionsComplete(bool Successful)
{
	TriggerOnSessionFindCanceledDelegates(Successful);
}

/*void UStalkerMPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	//TriggerOnJoinServerCompleteDelegates(SessionName, ResultType);
}*/

void UStalkerMPGameInstance::OnNetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString)
{
	TriggerOnNetworkFailureDelegates(FailureType, ErrorString);
}

void UStalkerMPGameInstance::JoinServer(FString SessionId)
{
	if (!ConnectedSessionName.Equals(""))
	{
		DisconnectFromServer();
	}

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		for (FOnlineSessionSearchResult SearchResult : OnlineSessionSearch.Get()->SearchResults)
		{
			if (SearchResult.GetSessionIdStr().Equals(SessionId))
			{
				FString SessionName;
				SearchResult.Session.SessionSettings.Get<FString>(TEXT("SessionName"), SessionName);

				SessionInt->JoinSession(0, FName(*SessionName), SearchResult);

				break;
			}
		}
	}
}

void UStalkerMPGameInstance::CancelFindServers()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		SessionInt->CancelFindSessions();
	}
}

void UStalkerMPGameInstance::DisconnectFromServer()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();

		if (!ConnectedSessionName.Equals(""))
		{
			SessionInt->DestroySession(FName(*ConnectedSessionName));
		}
	}

	ConnectedSessionName = "";
}
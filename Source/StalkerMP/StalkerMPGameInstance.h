// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataClasses/GlobalEnums.h"

#include "Engine/GameInstance.h"
#include "OnlineDelegateMacros.h"
#include "StalkerMPGameInstance.generated.h"

class UServerData;

class UNetDriver;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFloatSettingValueUpdatedDelegate, FString, Key, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntSettingValueUpdatedDelegate, FString, Key, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStringSettingValueUpdatedDelegate, FString, Key, FString, Value);

/**
 * 
 */
UCLASS()
class STALKERMP_API UStalkerMPGameInstance : public UGameInstance
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

	virtual void Init() override;
	virtual void Shutdown() override;




// ######################################################################### User Settings #########################################################################

// ------------------------------------------------------------------------- Delegates --------------------------------------------------------------------------

public:

	static FOnFloatSettingValueUpdatedDelegate OnFloatSettingValueUpdatedDelegate;
	static FOnIntSettingValueUpdatedDelegate OnIntSettingValueUpdatedDelegate;
	static FOnStringSettingValueUpdatedDelegate OnStringSettingValueUpdatedDelegate;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		TMap<FString, FString> Map_GeneralSettings;

	UPROPERTY()
		TMap<FString, FString> Map_VideoSettings;

	UPROPERTY()
		TMap<FString, FString> Map_SoundSettings;

	UPROPERTY()
		TMap<FString, FString> Map_ControlsSettings;

	UPROPERTY()
		TMap<FString, FString> Map_ServerSettings;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString SaveDirectory = FString("../../Config");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString GeneralSettingsFileName = FString("General.cfg");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString VideoSettingsFileName = FString("Video.cfg");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString SoundSettingsFileName = FString("Sound.cfg");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString ControlsSettingsFileName = FString("Controls.cfg");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString ServerSettingsFileName = FString("Server.cfg");




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void LoadAllSettings();

	UFUNCTION()
		void LoadSettings(ESettingsType SettingsType);

	UFUNCTION()
		void PutSettingValue(ESettingsType SettingsType, FString Key, FString Value);

	UFUNCTION()
		FString ParseLaunchOption(FString Key, FString Fallback);



public:

	UFUNCTION(BlueprintCallable)
		void SaveSettings(ESettingsType SettingsType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetFloatSettingValue(ESettingsType SettingsType, FString Key, float Fallback);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetIntSettingValue(ESettingsType SettingsType, FString Key, int Fallback);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetStringSettingValue(ESettingsType SettingsType, FString Key, FString fallback);

	UFUNCTION(BlueprintCallable)
		void RemoveSetting(ESettingsType SettingsType, FString Key);

	UFUNCTION(BlueprintCallable)
		void PutFloatSettingValue(ESettingsType SettingsType, FString Key, float Value);

	UFUNCTION(BlueprintCallable)
		void PutIntSettingValue(ESettingsType SettingsType, FString Key, int Value);

	UFUNCTION(BlueprintCallable)
		void PutStringSettingValue(ESettingsType SettingsType, FString Key, FString Value);




// ------------------------------------------------------------------------- Delegates --------------------------------------------------------------------------

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, TArray<UServerData*>);
	typedef FOnSessionsFound::FDelegate FOnSessionsFoundDelegate;
	DEFINE_ONLINE_DELEGATE_ONE_PARAM(OnSessionsFound, TArray<UServerData*>);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionFindCanceled, bool);
	typedef FOnSessionFindCanceled::FDelegate FOnSessionFindCanceledDelegate;
	DEFINE_ONLINE_DELEGATE_ONE_PARAM(OnSessionFindCanceled, bool);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSessionJoinComplete, FString, EOnSessionJoinResult);
	typedef FOnSessionJoinComplete::FDelegate FOnSessionJoinCompleteDelegate;
	DEFINE_ONLINE_DELEGATE_TWO_PARAM(OnSessionJoinComplete, FString, EOnSessionJoinResult);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNetworkFailure, ENetworkFailure::Type, FString);
	typedef FOnNetworkFailure::FDelegate FOnNetworkFailureDelegate;
	DEFINE_ONLINE_DELEGATE_TWO_PARAM(OnNetworkFailure, ENetworkFailure::Type, FString);




	// ######################################################################### Online Subsystem #########################################################################

// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

	static const FString SERVER_NAME_SETTING_KEY;
	static const FString DEFAULT_SERVER_NAME;
	static const FString MAX_PLAYERS_SETTING_KEY;
	static const int DEFAULT_MAX_PLAYERS;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FString ConnectedSessionName = "";

	UPROPERTY()
		FString TheSessionName;

	UPROPERTY()
		FString CurrentGameMode;

	UPROPERTY()
		FString CurrentMapName;


	//UPROPERTY()
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnCancelFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	FDelegateHandle OnNetworkFailureDelegateHandle;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	//UPROPERTY()
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		FString GetGameVersion();

	UFUNCTION()
		void InitOnlineSubsystem();

	UFUNCTION()
		void DestroyOnlineSubsystem();

	UFUNCTION()
		void CreateSession();

	UFUNCTION()
		void OnDestroySessionComplete(FName SessionName, bool Successful);

	UFUNCTION()
		void OnFindSessionsComplete(bool Successful);

	UFUNCTION()
		void OnCancelFindSessionsComplete(bool Successful);

	// For some reason EOnJoinSessionCompleteResult isn't visible. Have to use Lambda instead
	/*UFUNCTION()
		void OnJoinSessionComplete(FName SessionName, const EOnJoinSessionCompleteResult::Type ResultType);*/

	UFUNCTION()
		void OnNetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);


public:

	UFUNCTION()
		void UpdateSession(FString GameMode, FString MapName);

	UFUNCTION()
		void FindServers();

	UFUNCTION()
		void JoinServer(FString SessionId);

	UFUNCTION()
		void CancelFindServers();

	UFUNCTION()
		void DisconnectFromServer();

};



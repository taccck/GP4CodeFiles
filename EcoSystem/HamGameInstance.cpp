#include "HamGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"

UHamGameInstance::UHamGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameInstance = this;
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UHamGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UHamGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UHamGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UHamGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UHamGameInstance::OnDestroySessionComplete);
}

FString UHamGameInstance::GetNetId() const
{
	return GetFirstGamePlayer()->GetPreferredUniqueNetId().GetV1()->ToString();
}

bool UHamGameInstance::HostHamSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, 
                                      bool bIsPresence, FString ServerName)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr || !UserId.IsValid())
	{
		return false;
	}
	
	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = bIsLAN;
	SessionSettings->bUsesPresence = bIsPresence;
	SessionSettings->NumPublicConnections = PlayerCount;
	SessionSettings->NumPrivateConnections = 0;
	SessionSettings->bAllowInvites = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

	SessionSettings->Set(SETTING_MAPNAME, ServerName, EOnlineDataAdvertisementType::ViaOnlineService);
	
	OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	return Sessions->CreateSession(*UserId, NAME_GameSession, *SessionSettings);
}

void UHamGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}

	Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	if (bWasSuccessful)
	{
		OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
		Sessions->StartSession(SessionName);
	}
}

void UHamGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}

	Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "OnlineLobby", true, "listen");
	}	
}

void UHamGameInstance::FindHamSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr || !UserId.IsValid())
	{
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->bIsLanQuery = bIsLAN;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->PingBucketSize = 50;
	if (bIsPresence)
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
	}

	const TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	Sessions->FindSessions(*UserId, SearchSettingsRef);
}

void UHamGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}

	Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

	if (SessionSearch->SearchResults.Num() > 0)
	{
		ServerNames.Empty();
		for (size_t i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			FString ServerName;
			SessionSearch->SearchResults[i].Session.SessionSettings.Get(SETTING_MAPNAME, ServerName);
			ServerNames.Add(ServerName);
		}
	}	
	ReceivedFinishedFind();
}

bool UHamGameInstance::JoinHamSession(TSharedPtr<const FUniqueNetId> UserId, 
	const FOnlineSessionSearchResult& SearchResult)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr || !UserId.IsValid())
	{
		return false;
	}
	
	OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	return Sessions->JoinSession(*UserId, NAME_GameSession, SearchResult);

}

void UHamGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}
	
	Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	
	APlayerController* const PlayerController = GetFirstLocalPlayerController();
	FString TravelURL;
	if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
	{
		PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}

void UHamGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}

	Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true);
	}
}

void UHamGameInstance::HostGame(FString ServerName)
{
	const ULocalPlayer* Player = GetFirstGamePlayer();
	HostHamSession(Player->GetPreferredUniqueNetId().GetV1(), true, true, ServerName);
}

void UHamGameInstance::FindGame()
{
	const ULocalPlayer* Player = GetFirstGamePlayer();
	FindHamSessions(Player->GetPreferredUniqueNetId().GetV1(), true, true);
}

void UHamGameInstance::JoinGame()
{
	const ULocalPlayer* const Player = GetFirstGamePlayer();

	if (!SessionSearch.IsValid())
	{
		return;
	}

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			if(SessionSearch->SearchResults[i].IsValid())
			{
				if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
				{
					const FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[i];
					JoinHamSession(Player->GetPreferredUniqueNetId().GetV1(), SearchResult);
					break;
				}
			}
		}
	}
}

void UHamGameInstance::DestroyGame()
{
	const IOnlineSessionPtr Sessions = GetSession();
	if (Sessions == nullptr)
	{
		return;
	}

	Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
	Sessions->DestroySession(NAME_GameSession);
}

IOnlineSessionPtr UHamGameInstance::GetSession()
{
	if (const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface(); Sessions.IsValid())
		{
			return Sessions;
		}
	}
	return nullptr;
}

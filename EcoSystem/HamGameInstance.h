#pragma once

#include "Interfaces/OnlineSessionInterface.h"
#include "HamGameInstance.generated.h"

UCLASS(abstract)
class UHamGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UHamGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, meta = (DisplayName="HamGameInstance"))
	static UHamGameInstance* Get() {return GameInstance;}

	UFUNCTION(BlueprintPure)
	FString GetNetId() const;

	/*Create a session as the host*/
	bool HostHamSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, FString ServerName);
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/*Find session and stores them in SessionSearch*/
	void FindHamSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	void OnFindSessionsComplete(bool bWasSuccessful);

	/*Join a session from search results*/
	bool JoinHamSession(TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult);
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/*Destroy current hosted session*/
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Networking")
	void HostGame(FString ServerName);

	UFUNCTION(BlueprintCallable, Category = "Networking")
	void FindGame();
	
	UFUNCTION(BlueprintCallable, Category = "Networking")
	void JoinGame();
	
	UFUNCTION(BlueprintCallable, Category = "Networking")
	void DestroyGame();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "FinishedFind"))
	void ReceivedFinishedFind();
	
private:
	static IOnlineSessionPtr GetSession();
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString MapToPlay;
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerCount;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> ServerNames;
	UPROPERTY(BlueprintReadWrite)
	bool bUseBots = false;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = "Stupid";
	UPROPERTY(BlueprintReadWrite)
	float MouseSensitivity = 1.f;
	UPROPERTY(BlueprintReadWrite)
	bool bInvertUpAxis = false;
	
private:
	inline static UHamGameInstance* GameInstance = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TetrisGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class TETRIS_API UTetrisGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTetrisGameInstance();

	virtual void Init() override;
	
	void Login();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(BlueprintCallable)
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void FindSession();
	void OnFindSessionComplete(bool bWasSuccessful);
	TSharedPtr<class FOnlineSessionSearch> SearchSettings;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	UFUNCTION(BlueprintCallable)
	void GetAllFriends();
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(BlueprintCallable)
	void ShowFriendsUI();

	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();

	UFUNCTION(BlueprintCallable)
	void SetMainMenuUI(class UTetrisMainMenu* Widget);
	
protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn = false;

	UTetrisMainMenu* MainMenuUI;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisGameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Tetris/HUD/TetrisMainMenu.h"


const FName TetrisSessionName = FName("Tetris Session");

UTetrisGameInstance::UTetrisGameInstance()
{
}

void UTetrisGameInstance::Init()
{
	Super::Init();
	
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is Valid"));
	}
	Login();
}

void UTetrisGameInstance::Login()
{
	UE_LOG(LogTemp, Warning, TEXT("Logging"));

	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("127.0.0.1:8081");
			Credentials.Token = FString("keksh");
			Credentials.Type = FString("developer");

			Identity->OnLoginCompleteDelegates->AddUObject(this, &UTetrisGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
			UE_LOG(LogTemp, Warning, TEXT("To LoginComplete"));

		}
	}
}

void UTetrisGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	bIsLoggedIn = bWasSuccessful;
	UE_LOG(LogTemp, Warning, TEXT("LoggedIn: %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
			
			if (MainMenuUI)
			{
				MainMenuUI->EnableAllButtons();
			}
		}
	}
}

void UTetrisGameInstance::CreateSession()
{
	IOnlineSessionPtr SessionPtr = nullptr;

	if (bIsLoggedIn &&
		OnlineSubsystem &&
		(SessionPtr = OnlineSubsystem->GetSessionInterface()) != nullptr)
	{

		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;

		SessionSettings.Set(SEARCH_KEYWORDS, FString("keksh"), EOnlineDataAdvertisementType::ViaOnlineService);

		SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UTetrisGameInstance::OnCreateSessionComplete);
		SessionPtr->CreateSession(0, TetrisSessionName, SessionSettings);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Session: Not Logged In"));
	}
}

void UTetrisGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateSession Success: %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			UWorld* World = GetWorld();
			if (World)
			{
				World->ServerTravel("/Game/Maps/LobbyMap?listen");
			}
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}
}

void UTetrisGameInstance::DestroySession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UTetrisGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(TetrisSessionName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Destroy Session: Not Logged In"));
	}
}

void UTetrisGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}

void UTetrisGameInstance::FindSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("keksh"), EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UTetrisGameInstance::OnFindSessionComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());
			}
		}
	}
}

void UTetrisGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("FindSession Success: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d Lobbies"), SearchSettings->SearchResults.Num());

		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				if (SearchSettings->SearchResults.Num())
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UTetrisGameInstance::OnJoinSessionComplete);
					SessionPtr->JoinSession(0, TetrisSessionName, SearchSettings->SearchResults[0]);
				}
			}
		}
	}
}

void UTetrisGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// && Result == EOnJoinSessionCompleteResult::Success

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			if (SearchSettings->SearchResults.Num())
			{
				FString ConnectionInfo = FString();
				SessionPtr->GetResolvedConnectString(SessionName, ConnectionInfo);
				if (!ConnectionInfo.IsEmpty())
				{
					if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
					{
						PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
}

void UTetrisGameInstance::GetAllFriends()
{
	IOnlineFriendsPtr FriendsPtr = nullptr;

	if (bIsLoggedIn && 
		OnlineSubsystem &&
		(FriendsPtr = OnlineSubsystem->GetFriendsInterface()) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to get FriendsList"));
		FriendsPtr->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UTetrisGameInstance::OnGetAllFriendsComplete));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Was not Successful At Getting FriendsList"));
	}
}

void UTetrisGameInstance::OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString & ListName, const FString & ErrorStr)
{
	if (OnlineSubsystem)
	{
		if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
		{
			UE_LOG(LogTemp, Warning, TEXT("Was Successful At Getting FriendsList"));

			TArray<TSharedRef<FOnlineFriend>> FriendsList;
			if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
			{
				for (TSharedRef<FOnlineFriend> Friend : FriendsList)
				{
					FString FriendName = Friend.Get().GetRealName();
					UE_LOG(LogTemp, Warning, TEXT("Friend: %s"), *FriendName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed GetFriendsList"));
			}
		}
	}
}

void UTetrisGameInstance::ShowFriendsUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowFriendsUI(0);
			}
		}
	}
}

void UTetrisGameInstance::ShowInviteUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowInviteUI(0, TetrisSessionName);
			}
		}
	}
}

void UTetrisGameInstance::SetMainMenuUI(UTetrisMainMenu* PlayerWidget)
{
	MainMenuUI = PlayerWidget;

	if (bIsLoggedIn)
	{
		if (MainMenuUI)
		{
			MainMenuUI->EnableAllButtons();
		}
	}
}

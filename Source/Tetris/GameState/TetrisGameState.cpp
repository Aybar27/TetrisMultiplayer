// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris/GameState/TetrisGameState.h"
#include "Net/UnrealNetwork.h"
#include "Tetris/PlayerState/TetrisPlayerState.h"

void ATetrisGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATetrisGameState, PlayersPoints); 
	DOREPLIFETIME(ATetrisGameState, PlayersIsOver); 
}

void ATetrisGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	PlayersPoints.Add(0);
	PlayersIsOver.Add(false);
}

void ATetrisGameState::AddPlayersCount(ATetrisPlayerState* PlayerState, int32 NumberOfLinesDestroyed)
{
	TetrisPlayerState = PlayerState;

	int32 ArrayIndex;
	ATetrisPlayerState* OpponentPlayerState = nullptr;

	bool bNotReturn =
		PlayerState != nullptr &&
		IsValid(TetrisPlayerState = PlayerState) &&
		(ArrayIndex = PlayerArray.Find(TetrisPlayerState)) != -1 &&
		(OpponentPlayerState = Cast<ATetrisPlayerState>(PlayerArray[(ArrayIndex == 0 ? 1 : 0)])) != nullptr;

	if (!bNotReturn) return;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: GameState_AddPlayersCount"));

	ArrayIndex = PlayerArray.Find(TetrisPlayerState);

	int32* PlayerPoints = &PlayersPoints[ArrayIndex];
	int32* OpponentPoints = &PlayersPoints[(ArrayIndex == 0 ? 1 : 0)];

	*PlayerPoints += NumberOfLinesDestroyed * NumberOfLinesDestroyed * PointsForLine;

	TetrisPlayerState->SetPlayerPoints(*PlayerPoints);
	TetrisPlayerState->SetOpponentPoints(*OpponentPoints);
	
	OpponentPlayerState->SetPlayerPoints(*OpponentPoints);
	OpponentPlayerState->SetOpponentPoints(*PlayerPoints);

	if (PlayersIsOver[(ArrayIndex == 0 ? 1 : 0)])
	{
		if (*OpponentPoints < *PlayerPoints)
		{
			SetGameIsOver(true, TetrisPlayerState);
			return;
		}
	}

	TetrisPlayerState->SetGameSpeed(*PlayerPoints / NextLevelPoints + 1);

	CountPointsForBlock += *PlayerPoints;
	if (CountPointsForBlock >= PointsForBlock)
	{
		CountPointsForBlock -= PointsForBlock;

		OpponentPlayerState->SpawnBlocks();
	}
}

void ATetrisGameState::SetGameIsOver(bool bIsWinning, ATetrisPlayerState* PlayerState)
{
	if (PlayerState == nullptr) return;
	TetrisPlayerState = PlayerState;

	int32 ArrayIndex;
	ATetrisPlayerState* OpponentPlayerState = nullptr;

	bool bNotReturn = TetrisPlayerState &&
		!PlayerArray.IsEmpty() && !PlayersIsOver.IsEmpty() && !PlayersPoints.IsEmpty() &&
		(ArrayIndex = PlayerArray.Find(TetrisPlayerState)) != -1 &&
		(OpponentPlayerState = Cast<ATetrisPlayerState>(PlayerArray[(ArrayIndex == 0 ? 1 : 0)])) != nullptr;

	if (!bNotReturn) return;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: GameState_SetGameIsOver"));

	PlayersIsOver[ArrayIndex] = true;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 1")));

	if (bIsWinning)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 2.1")));

		TetrisPlayerState->SetGameIsWinning();
		OpponentPlayerState->SetGameIsOver();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 2.2")));

		int32 PlayerPoints = 0;
		int32 OpponentPoints = 0;

		PlayerPoints = PlayersPoints[ArrayIndex];
		OpponentPoints = PlayersPoints[(ArrayIndex == 0 ? 1 : 0)];
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PlayePoints: %i"), PlayerPoints));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OpponentPoints: %i"), OpponentPoints));
		bool bOpponentIsOver = PlayersIsOver[(ArrayIndex == 0 ? 1 : 0)];
		if (bOpponentIsOver)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 3.1")));

			if (PlayerPoints >= OpponentPoints)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 4.1")));

				TetrisPlayerState->SetGameIsWinning();
				OpponentPlayerState->SetGameIsOver();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 4.2")));

				TetrisPlayerState->SetGameIsOver();
				OpponentPlayerState->SetGameIsWinning();
			}
		}

		else if (PlayerPoints < OpponentPoints)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(TEXT("Moment 3.2")));

			TetrisPlayerState->SetGameIsOver();
			OpponentPlayerState->SetGameIsWinning();
		}
	}
}

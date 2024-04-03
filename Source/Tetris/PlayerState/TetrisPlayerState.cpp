// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris/PlayerState/TetrisPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Tetris/GameState/TetrisGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Tetris/Character/TetrisCharacter.h"

void ATetrisPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATetrisPlayerState, TetrisGameState);
}

void ATetrisPlayerState::SetOwnerCharacter(ATetrisCharacter* OwnerCharacter)
{
	if (!OwnerCharacter) return;
	TetrisOwnerCharacter = OwnerCharacter;
}

void ATetrisPlayerState::SendPlayersCount(int32 NumberOfLinesDestroyed)
{
	ServerSendPlayersCount(NumberOfLinesDestroyed);
}

void ATetrisPlayerState::ServerSendPlayersCount_Implementation(int32 NumberOfLinesDestroyed)
{
	TetrisGameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(this));

	if (!TetrisGameState) return;
	TetrisGameState->AddPlayersCount(this, NumberOfLinesDestroyed);
}

bool ATetrisPlayerState::ServerSendPlayersCount_Validate(int32 NumberOfLinesDestroyed)
{
	return NumberOfLinesDestroyed <= 4 && NumberOfLinesDestroyed >= 0;
}

void ATetrisPlayerState::SetGameSpeed(int32 Speed)
{
	if (Speed <= 1) return;
	MulticastSetGameSpeed(Speed);
}

void ATetrisPlayerState::MulticastSetGameSpeed_Implementation(int32 Speed)
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SetGameSpeed(Speed);
}

void ATetrisPlayerState::SpawnBlocks()
{
	MulticastSpawnBlocks();
}

void ATetrisPlayerState::MulticastSpawnBlocks_Implementation()
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SpawnBlocks();
}

void ATetrisPlayerState::SendGameEnding(bool bIsWin)
{
	ServerSendGameEnding(bIsWin);
}

void ATetrisPlayerState::ServerSendGameEnding_Implementation(bool bIsWin)
{
	TetrisGameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(this));

	if (!TetrisGameState) return;
	TetrisGameState->SetGameIsOver(bIsWin, this);
}

void ATetrisPlayerState::SetGameIsOver()
{
	MulticastSetGameIsOver();
}

void ATetrisPlayerState::MulticastSetGameIsOver_Implementation()
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SetGameIsOver();
}

void ATetrisPlayerState::SetGameIsWinning()
{
	MulticastSetGameIsWinning();
}

void ATetrisPlayerState::MulticastSetGameIsWinning_Implementation()
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SetGameIsWinning();
}

void ATetrisPlayerState::SetPlayerPoints(int32 Points)
{
	MulticastSetPlayerPoints(Points);
}

void ATetrisPlayerState::SetOpponentPoints(int32 Points)
{
	MulticastSetOpponentPoints(Points);
}

void ATetrisPlayerState::MulticastSetPlayerPoints_Implementation(int32 Points)
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SetPlayerPoints(Points);
}

void ATetrisPlayerState::MulticastSetOpponentPoints_Implementation(int32 Points)
{
	if (!TetrisOwnerCharacter) return;
	TetrisOwnerCharacter->SetOpponentPoints(Points);
}

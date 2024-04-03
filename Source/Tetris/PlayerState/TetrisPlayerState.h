// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TetrisPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SetOwnerCharacter(class ATetrisCharacter* OwnerCharacter);

	UFUNCTION()
	void SendPlayersCount(int32 NumberOfLinesDestroyed);

	UFUNCTION()
	void SetGameSpeed(int32 Speed);

	UFUNCTION()
	void SpawnBlocks();

	UFUNCTION()
	void SendGameEnding(bool bIsWin);

	UFUNCTION()
	void SetGameIsOver();

	UFUNCTION()
	void SetGameIsWinning();

	UFUNCTION()
	void SetPlayerPoints(int32 Points);

	UFUNCTION()
	void SetOpponentPoints(int32 Points);

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendPlayersCount(int32 NumberOfLinesDestroyed);

	UFUNCTION(NetMulticast, reliable)
	void MulticastSetGameSpeed(int32 Speed);

	UFUNCTION(NetMulticast, reliable)
	void MulticastSpawnBlocks();

	UFUNCTION(Server, Reliable)
	void ServerSendGameEnding(bool bIsWin);

	UFUNCTION(NetMulticast, reliable)
	void MulticastSetGameIsOver();

	UFUNCTION(NetMulticast, reliable)
	void MulticastSetGameIsWinning();

	UFUNCTION(NetMulticast, reliable)
	void MulticastSetPlayerPoints(int32 Points);

	UFUNCTION(NetMulticast, reliable)
	void MulticastSetOpponentPoints(int32 Points);

	UPROPERTY(BlueprintReadOnly, replicated)
	class ATetrisGameState* TetrisGameState;

	UPROPERTY(BlueprintReadOnly)
	ATetrisCharacter* TetrisOwnerCharacter;
	
};

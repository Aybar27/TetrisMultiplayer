// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TetrisGameState.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState);

	UFUNCTION()
	void AddPlayersCount(class ATetrisPlayerState* PlayerState, int32 NumberOfLinesDestroyed);

	UFUNCTION()
	void SetGameIsOver(bool bIsWinning, ATetrisPlayerState* PlayerState);

protected:
	UPROPERTY(Replicated)
	TArray<int32> PlayersPoints;

	UPROPERTY(Replicated)
	TArray<bool> PlayersIsOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsForLine = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NextLevelPoints = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsForBlock = 100;

	UPROPERTY(BlueprintReadOnly)
	int32 CountPointsForBlock = 0;

	UPROPERTY(BlueprintReadOnly)
	class ATetrisGameMode* TetrisGameMode;

	UPROPERTY(BlueprintReadOnly)
	ATetrisPlayerState* TetrisPlayerState;
};

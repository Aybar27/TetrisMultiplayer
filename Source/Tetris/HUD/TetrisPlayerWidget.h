// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TetrisPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayerPoints(int32 Points);
	
	UFUNCTION()
	void SetOpponentPoints(int32 Points);

	UFUNCTION()
	void SetWinningText(bool bIsWon);

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayersPointsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OpponentsPointsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinningText;

};


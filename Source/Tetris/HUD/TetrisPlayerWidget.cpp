// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris/HUD/TetrisPlayerWidget.h"
#include "Components/TextBlock.h"

bool UTetrisPlayerWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UTetrisPlayerWidget::SetPlayerPoints(int32 Points)
{
	FString PointsString = FString::FromInt(Points);
	PlayersPointsText->SetText(FText::FromString(PointsString));
}

void UTetrisPlayerWidget::SetOpponentPoints(int32 Points)
{
	FString PointsString = FString::FromInt(Points);
	OpponentsPointsText->SetText(FText::FromString(PointsString));
}

void UTetrisPlayerWidget::SetWinningText(bool bIsWon)
{
	WinningText->SetVisibility(ESlateVisibility::Visible);
	if (!bIsWon)
	{
		WinningText->SetText(FText::FromString(TEXT("YOU LOST")));
		WinningText->SetColorAndOpacity(FLinearColor::Red);
	}
}


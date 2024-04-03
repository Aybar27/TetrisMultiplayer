// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris/HUD/TetrisMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CircularThrobber.h"

bool UTetrisMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UTetrisMainMenu::EnableAllButtons()
{
	HostButton->SetIsEnabled(true);
	JoinButton->SetIsEnabled(true);
	ShowFriendsListButton->SetIsEnabled(true);
	ShowInviteButton->SetIsEnabled(true);
	LoggingInThrobber->SetVisibility(ESlateVisibility::Hidden);
	LoggingInText->SetText(FText::FromString(TEXT("Logged In Successfully")));
}

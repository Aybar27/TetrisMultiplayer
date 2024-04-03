// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TetrisMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void EnableAllButtons();

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ShowFriendsListButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ShowInviteButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoggingInText;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* LoggingInThrobber;
};

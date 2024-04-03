
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TetrisCharacter.generated.h"

//Class to move figures
UCLASS()
class TETRIS_API ATetrisCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATetrisCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetGameSpeed(int32 Speed);

	UFUNCTION()
	void SpawnBlocks();

	UFUNCTION()
	void SetGameIsOver();

	UFUNCTION()
	void SetGameIsWinning();

	UFUNCTION()
	void SetPlayerPoints(int32 Points);

	UFUNCTION()
	void SetOpponentPoints(int32 Points);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf <class UTetrisPlayerWidget> TetrisPlayerWidget;

	UPROPERTY(BlueprintReadOnly)
	UTetrisPlayerWidget* PlayerWidget;

	// Tetris Figures
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf <class ATetrisFigures> TetrisFigureClass;

	UPROPERTY(BlueprintReadOnly)
	ATetrisFigures* TetrisFigure;

	UPROPERTY(BlueprintReadOnly)
	class ATetrisFiguresActor* SpawnedFigure;

	// Move Functions
	void MoveDownFast();
	void MoveDownSlow();
	void MoveRight();
	void MoveLeft();
	void Rotate();

	// Other Classes
	UPROPERTY(BlueprintReadOnly)
	class ATetrisPlayerState* TetrisPlayerState;

	UPROPERTY(BlueprintReadOnly)
	class ATetrisGameState* TetrisGameState;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameSpeed = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameAccelaration = 0.04;

public:	


};

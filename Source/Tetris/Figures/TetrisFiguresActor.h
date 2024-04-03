
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tetris/Structs/TetrisVector2D.h"
#include "TetrisFiguresActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMoveDownComplete);

// Figures Movement Type
UENUM(BlueprintType)
enum class EMovementType :uint8
{
	Down,
	Right,
	Left,
	Rotate
};

//Tetris figures parent class
UCLASS()
class TETRIS_API ATetrisFiguresActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetrisFiguresActor();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// Delegates
	FOnMoveDownComplete OnMoveDownComplete;

	// Figures Move
	UFUNCTION()
	void SetTimerMoveDown(float Speed);

	UFUNCTION(BlueprintCallable)
	void MoveFiguresDown();

	UFUNCTION(BlueprintCallable)
	void MoveRight();

	UFUNCTION(BlueprintCallable)
	void MoveLeft();

	UFUNCTION(BlueprintCallable)
	void RotateFigure();

	// Game Parameters
	UFUNCTION(BlueprintCallable)
	void ChangeGameSpeed(float Speed);

	void SetFiguresGridPosition(TArray<FTetrisVector2D> Vector2D);

protected:

	// Figures Move
	UFUNCTION(BlueprintCallable)
	bool CheckCanMove(TArray<FTetrisVector2D>& TetrisFiguresLocation);

	UFUNCTION(BlueprintCallable)
	void MoveFigures(EMovementType Type);

	UFUNCTION(BlueprintCallable)
	FTetrisVector2D MoveFiguresChoose(EMovementType Type, FVector CurrentLocation, FTetrisVector2D OriginPoint);

	UFUNCTION(BlueprintCallable)
	bool CheckIsGameOver(TArray<FTetrisVector2D>& TetrisFiguresLocation);

	UPROPERTY(BlueprintReadWrite)
	TArray<bool> MovingDownBools{ false, false, false, false };

	FTimerHandle MovingDownTimer;


	// Figures
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* Figure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPaperSpriteComponent*> FiguresArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTetrisVector2D> FiguresGridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FigureName = "TetrisFigures_0";

	UPROPERTY()
	class ATetrisFigures* TetrisFigure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfTiles = 4;


	// Game Parameters
	float GameSpeed;

	UFUNCTION(BlueprintCallable)
	void DestroyLine();


private:

};

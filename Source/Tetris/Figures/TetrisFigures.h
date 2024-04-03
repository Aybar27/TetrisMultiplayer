
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Tetris/Structs/TetrisVector2D.h"
#include "TetrisFigures.generated.h"


//Class to spawn tetris figures
UCLASS()
class TETRIS_API ATetrisFigures : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual void BeginPlay() override;

	void SetPlayerState(class ATetrisPlayerState* PlayerState);
	void SetGameState(class ATetrisGameState* GameState);
	void SetGameSpeed(float Speed);
	void SetIsSpawnBlocks();

	FORCEINLINE float GetGameSpeed() const { return GameSpeed; }
	FORCEINLINE TArray<FTetrisVector2D>& GetOccupiedGridPositions() { return OccupiedGridPositions; }
	FORCEINLINE class ATetrisFiguresActor* GetSpawnedFigure() { return SpawnedFigure; }
	FORCEINLINE class TArray<UPaperSpriteComponent*>& GetFiguresTilesArray() { return TetrisFiguresTiles; }
	FORCEINLINE ATetrisPlayerState* GetPlayerState() { return TetrisPlayerState; }
	FORCEINLINE ATetrisGameState* GetGameState() { return TetrisGameState; }

	UFUNCTION(BlueprintCallable)
	void SpawnFigures();

	UFUNCTION(BlueprintCallable)
	int32 AddOccupiedGridPositions(FTetrisVector2D Vector2D);

	UFUNCTION(BlueprintCallable)
	int32 AddFiguresTiles(UPaperSpriteComponent* FigureTile);

	UFUNCTION(BlueprintCallable)
	void ClearLine(int YPosition);

	UFUNCTION(BlueprintCallable)
	void SpawnBlocks();

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperSpriteComponent> Figure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UPaperSpriteComponent>> FiguresArray;

	FTimerHandle MovingDownTimer;

	UPROPERTY(BlueprintReadOnly)
	float GameSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATetrisFiguresActor> TetrisFigureClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ATetrisFiguresActor>> TetrisFiguresArray;

	UPROPERTY(BlueprintReadOnly)
	ATetrisFiguresActor* SpawnedFigure;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTetrisVector2D> OccupiedGridPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPaperSpriteComponent*> TetrisFiguresTiles;

	UPROPERTY(BlueprintReadOnly)
	ATetrisPlayerState* TetrisPlayerState;

	UPROPERTY(BlueprintReadOnly)
	ATetrisGameState* TetrisGameState;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSpawnBlocks = false;

private:


};

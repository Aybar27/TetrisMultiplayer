
#include "TetrisFigures.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "TetrisFiguresActor.h"
#include "Tetris/PlayerState/TetrisPlayerState.h"
#include "Tetris/GameState/TetrisGameState.h"


//Class to spawn tetris figures
void ATetrisFigures::BeginPlay()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigure_Spawned"));

	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	int32 FiguresArrayNum = TetrisFiguresArray.Num() - 1;

	SpawnedFigure = GetWorld()->SpawnActor<ATetrisFiguresActor>(
		TetrisFiguresArray[UKismetMathLibrary::RandomInteger(FiguresArrayNum)], SpawnParams);

	if (!SpawnedFigure) return;

	SpawnedFigure->SetTimerMoveDown(GameSpeed);
	SpawnedFigure->OnMoveDownComplete.AddUObject(this, &ATetrisFigures::SpawnFigures);
	SpawnedFigure->OnMoveDownComplete.AddUObject(this, &ATetrisFigures::SpawnBlocks);
}

void ATetrisFigures::SetPlayerState(ATetrisPlayerState* PlayerState)
{
	if (PlayerState == nullptr) return;
	TetrisPlayerState = PlayerState;
}

void ATetrisFigures::SetGameState(ATetrisGameState* GameState)
{
	TetrisGameState = GameState;
	SpawnedFigure->SetTimerMoveDown(GameSpeed);
}

void ATetrisFigures::SetGameSpeed(float Speed)
{
	GameSpeed = Speed;
}

void ATetrisFigures::SpawnFigures()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigure_SpawnFigures"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	SpawnedFigure = GetWorld()->SpawnActor<ATetrisFiguresActor>(
		TetrisFiguresArray[UKismetMathLibrary::RandomInteger(7)], SpawnParams);

	if (!IsValid(SpawnedFigure)) return;

	SpawnedFigure->SetTimerMoveDown(GameSpeed);
	SpawnedFigure->OnMoveDownComplete.AddUObject(this, &ATetrisFigures::SpawnFigures);
	SpawnedFigure->OnMoveDownComplete.AddUObject(this, &ATetrisFigures::SpawnBlocks);
}

int32 ATetrisFigures::AddOccupiedGridPositions(FTetrisVector2D Vector2D)
{
	return OccupiedGridPositions.Add(Vector2D);
}

int32 ATetrisFigures::AddFiguresTiles(UPaperSpriteComponent* FigureTile)
{
	return TetrisFiguresTiles.Add(FigureTile);
}

void ATetrisFigures::ClearLine(int Y)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigure_ClearLine"));

	for (int32 x = 0; x < 10; ++x)
	{
		FTetrisVector2D CurrentLocationRemove{ x,Y };

		int32 RemovedIndex = OccupiedGridPositions.Find(CurrentLocationRemove);
		OccupiedGridPositions.Remove(CurrentLocationRemove);

		TetrisFiguresTiles[RemovedIndex]->DestroyComponent();
		TetrisFiguresTiles.RemoveAt(RemovedIndex);

		for (int32 y = Y + 1; y < 20; ++y)
		{
			FTetrisVector2D CurrentLocationChange{ x,y };
			int32 IndexAtArray = OccupiedGridPositions.Find(CurrentLocationChange);

			if (IndexAtArray != -1)
			{
				OccupiedGridPositions[IndexAtArray].Y--;
			}
		}
	}
}

void ATetrisFigures::SetIsSpawnBlocks()
{
	bIsSpawnBlocks = true;
}

void ATetrisFigures::SpawnBlocks()
{
	if (bIsSpawnBlocks)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigure_SpawnBlocks"));

		bIsSpawnBlocks = false;

		for (int y = 19; y >= 0; y--)
		{
			TArray<FTetrisVector2D> FreeGridPosition;

			for (int x = 1; x < 8; ++x)
			{
				FTetrisVector2D CurrentLocationForBlock{ x,y };

				if (OccupiedGridPositions.Find(FTetrisVector2D{ x, y }) != -1)
				{
					if ((OccupiedGridPositions.Find(FTetrisVector2D{ x - 1, y + 1 }) == -1) &&
						(OccupiedGridPositions.Find(FTetrisVector2D{ x, y + 1 }) == -1) &&
						(OccupiedGridPositions.Find(FTetrisVector2D{ x + 1, y + 1 }) == -1))
					{
						FreeGridPosition.Add(FTetrisVector2D{ x, y + 1 });
					}
				}
			}

			if (!FreeGridPosition.IsEmpty())
			{
				FTetrisVector2D BlockPosition = FreeGridPosition[UKismetMathLibrary::RandomInteger(FreeGridPosition.Num())];

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				ATetrisFiguresActor* SpawnedBlock = GetWorld()->SpawnActor<ATetrisFiguresActor>(TetrisFiguresArray[7], SpawnParams);

				if (SpawnedBlock)
				{
					TArray<FTetrisVector2D> FreeGridPositions;

					for (int i = -1; i < 2; ++i)
					{
						FreeGridPositions.Add(FTetrisVector2D{ BlockPosition.X + i, BlockPosition.Y });
						AddOccupiedGridPositions(FTetrisVector2D{ BlockPosition.X + i, BlockPosition.Y });
					}

					SpawnedBlock->SetFiguresGridPosition(FreeGridPositions);
				}

				break;
			}
		}
	}
	else
	{
		return;
	}
}

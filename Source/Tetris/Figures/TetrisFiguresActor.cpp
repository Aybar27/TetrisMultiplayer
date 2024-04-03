

#include "TetrisFiguresActor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "TimerManager.h"
#include "TetrisFigures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tetris/PlayerState/TetrisPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"


//Tetris figures parent class
ATetrisFiguresActor::ATetrisFiguresActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComponent);

	if (!FigureName.IsEmpty())
	{ 
		for (int32 i = 0; i < AmountOfTiles; ++i)
		{
			Figure = CreateDefaultSubobject<UPaperSpriteComponent>(*FString::Printf(TEXT("Figure_%i"), i));
			Figure->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Figure->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
			Figure->SetSprite(ConstructorHelpers::FObjectFinder<UPaperSprite>(*FString::Printf(TEXT("/Game/Assets/Figures/%s"), *FigureName)).Object);

			FiguresArray.Add(Figure);
		}
	}
}

void ATetrisFiguresActor::BeginPlay()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFiguresActor_Spawned"));

	Super::BeginPlay();
	
	if (Owner)
	{
		TetrisFigure = Cast<ATetrisFigures>(Owner);
	}

	for (int i = 0; i < AmountOfTiles; ++i)
	{
		FiguresArray[i]->SetRelativeLocation(FVector(FiguresGridPosition[i].X * 64.0f, FiguresGridPosition[i].Y * 64.0f, 2.0f));
		if (TetrisFigure)
		{
			TetrisFigure->AddFiguresTiles(FiguresArray[i]);
		}
	}
 }

void ATetrisFiguresActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATetrisFiguresActor::SetTimerMoveDown(float Speed)
{
	GameSpeed = Speed;

	GetWorldTimerManager().SetTimer(MovingDownTimer, this, &ATetrisFiguresActor::MoveFiguresDown, GameSpeed, true);
}

bool ATetrisFiguresActor::CheckCanMove(TArray<FTetrisVector2D>& TetrisFiguresLocation)
{
	for (int i = 0; i < AmountOfTiles; ++i)
	{
		if (TetrisFiguresLocation[i].X >= 10 ||
			TetrisFiguresLocation[i].X < 0 ||
			TetrisFiguresLocation[i].Y < 0 ||
			!(-1 == TetrisFigure->GetOccupiedGridPositions().Find(TetrisFiguresLocation[i])))
		{
			return true;
		}
	}
	
	return false;
}

bool ATetrisFiguresActor::CheckIsGameOver(TArray<FTetrisVector2D>& TetrisFiguresLocation)
{
	for (int i = 0; i < AmountOfTiles; ++i)
	{
		if (TetrisFiguresLocation[i].Y == 19)
		{
			return true;
		}
	}

	return false;
}

void ATetrisFiguresActor::MoveFigures(EMovementType Type)
{
	if (!TetrisFigure) return;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigureActor_MoveFigures"));

	TArray<FTetrisVector2D> NewLocation2D;

	// Set CurrentLocaiton array
	TArray<FVector> CurrentLocation;
	for (int i = 0; i < AmountOfTiles; ++i)
	{
		CurrentLocation.Add(FiguresArray[i]->GetRelativeLocation());
	}

	// For Figures Rotation central point
	FTetrisVector2D OriginPoint = FTetrisVector2D{ (int32)CurrentLocation[1].X / 64, (int32)CurrentLocation[1].Y / 64 };

	for (int i = 0; i < AmountOfTiles; ++i)
	{
		// Determine CurrentLocation of figure tile
		FTetrisVector2D CurrentLocation2D = FTetrisVector2D{ (int32)CurrentLocation[i].X / 64, (int32)CurrentLocation[i].Y / 64 };

		// Determine NewLocation of figure tile
		NewLocation2D.Add(MoveFiguresChoose(Type, CurrentLocation[i], OriginPoint));
	}

	// If NewLocaiton is the most below point
	bool bCantMove(false);
	bCantMove = CheckCanMove(NewLocation2D);

	// If can`t move SetRelativeLocation to TetrisFigures
	if (!bCantMove)
	{
		for (int i = 0; i < AmountOfTiles; i++)
		{
			FiguresArray[i]->SetRelativeLocation(FVector(NewLocation2D[i].X * 64.0f, NewLocation2D[i].Y * 64.0f, CurrentLocation[i].Z));
		}
	}

	// If Down stop the Timer and AddOccupiedGridPosition
	if (bCantMove && Type == EMovementType::Down)
	{
		for (int i = 0; i < AmountOfTiles; ++i)
		{
			if (TetrisFigure)
			{
				TetrisFigure->AddOccupiedGridPositions(FTetrisVector2D{ NewLocation2D[i].X, (++NewLocation2D[i].Y) });
			}
		}

		if (CheckIsGameOver(NewLocation2D))
		{
			GetWorldTimerManager().ClearTimer(MovingDownTimer);

			if (TetrisFigure)
			{
				TetrisFigure->GetPlayerState()->SendGameEnding(false);
			}

			return;
		}

		DestroyLine();

		GetWorldTimerManager().ClearTimer(MovingDownTimer);

		if (TetrisFigure)
		{
			OnMoveDownComplete.Broadcast();
		}

	}
}

FTetrisVector2D ATetrisFiguresActor::MoveFiguresChoose(EMovementType Type, FVector CurrentLocation, FTetrisVector2D OriginPoint)
{
	switch (Type)
	{
		case EMovementType::Down:
		{
			return FTetrisVector2D{ (int32)CurrentLocation.X / 64, (int32)(CurrentLocation.Y - 64) / 64 };
		}
		case EMovementType::Right:
		{
			return FTetrisVector2D{ (int32)(CurrentLocation.X - 64) / 64, (int32)CurrentLocation.Y / 64 };
		}
		case EMovementType::Left:
		{
			return FTetrisVector2D{ (int32)(CurrentLocation.X + 64) / 64, (int32)CurrentLocation.Y / 64 };
		}
		case EMovementType::Rotate:
		{
			return FTetrisVector2D{ OriginPoint.X - ((int32)(CurrentLocation.Y / 64 - OriginPoint.Y)),
				OriginPoint.Y + ((int32)(CurrentLocation.X / 64 - OriginPoint.X)) };
		}
		default:break;
	}

	return FTetrisVector2D{ 0,0 };
}

void ATetrisFiguresActor::MoveFiguresDown()
{
	MoveFigures(EMovementType::Down);
}

void ATetrisFiguresActor::MoveRight()
{
	MoveFigures(EMovementType::Right);
}

void ATetrisFiguresActor::MoveLeft()
{
	MoveFigures(EMovementType::Left);
}

void ATetrisFiguresActor::RotateFigure()
{
	MoveFigures(EMovementType::Rotate);
}

void ATetrisFiguresActor::ChangeGameSpeed(float Speed)
{
	if (Speed < GameSpeed)
	{
		float TimerRate = GetWorldTimerManager().GetTimerRate(MovingDownTimer);
		double ErrorTolerance = 0.01;

		if (UKismetMathLibrary::NearlyEqual_FloatFloat((double)TimerRate, (double)(TimerRate - GetWorldTimerManager().GetTimerElapsed(MovingDownTimer)), ErrorTolerance))
		{
		}
		else
		{
			SetTimerMoveDown(Speed);
		}
	}
	else
	{
		SetTimerMoveDown(Speed);
	}
}

void ATetrisFiguresActor::SetFiguresGridPosition(TArray<FTetrisVector2D> Vector2D)
{
	if (FiguresGridPosition.Num() == AmountOfTiles)
	{
		for (int i = 0; i < AmountOfTiles; ++i)
		{
			FiguresGridPosition[i] = Vector2D[i];
			FiguresArray[i]->SetRelativeLocation(FVector(FiguresGridPosition[i].X * 64.0f, FiguresGridPosition[i].Y * 64.0f, 2.0f));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FiguresGridPosition Doesnt equal to AmountOfTiles"));
	}
}

void ATetrisFiguresActor::DestroyLine()
{
	if (!TetrisFigure) return;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: TetrisFigureActor_DestroyLine"));

	auto& OccupiedGridPosition = TetrisFigure->GetOccupiedGridPositions();

	int NumberOfDestroyedLines = 0;
	for (int y = 0; y < 20; ++y)
	{
		int NumberOfOnLineFigures = 0;
		for (int x = 0; x < 10; ++x)
		{
			FTetrisVector2D CurrentLocationCount{ x,y };
			if (!(-1 == OccupiedGridPosition.Find(CurrentLocationCount))) ++NumberOfOnLineFigures;
		}

		if (NumberOfOnLineFigures == 10)
		{
			++NumberOfDestroyedLines;
			TetrisFigure->ClearLine(y);

			for (int yy = 0; yy < 20; ++yy)
			{
				for (int xx = 0; xx < 10; ++xx)
				{
					FTetrisVector2D CurrentLocationCount{ xx,yy };
					int32 IndexAtArray = OccupiedGridPosition.Find(CurrentLocationCount);

					if (IndexAtArray != -1)
					{
						FVector NewLocation{ xx * 64.0f, yy * 64.0f, 1.0f };

						UPaperSpriteComponent* TetrisTile = TetrisFigure->GetFiguresTilesArray()[IndexAtArray];
						if (TetrisTile)
						{
							TetrisTile->SetRelativeLocation(NewLocation);
						}
					}
				}
			}

			--y;
		}
	}

	if (NumberOfDestroyedLines > 0)
	{
		if (!TetrisFigure) return;
		TetrisFigure->GetPlayerState()->SendPlayersCount(NumberOfDestroyedLines);
	}
}
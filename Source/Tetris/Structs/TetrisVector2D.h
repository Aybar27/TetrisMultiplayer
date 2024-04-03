#pragma once

#include "CoreMinimal.h"
#include "TetrisVector2D.generated.h"

//Structure for X and Y of tiles
USTRUCT(BlueprintType)
struct FTetrisVector2D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y = 0;

	bool operator== (FTetrisVector2D Vector2D)
	{
		return Vector2D.X == X && Vector2D.Y == Y;
	}

	bool operator== (const FTetrisVector2D Vector2D) const
	{
		return Vector2D.X == X && Vector2D.Y == Y;
	}
};
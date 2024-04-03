
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "TetrisGrid.generated.h"

//Tetris Grid 
UCLASS()
class TETRIS_API ATetrisGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetrisGrid();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* Tile;


public:	


};

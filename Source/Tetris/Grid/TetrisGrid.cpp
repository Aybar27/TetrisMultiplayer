

#include "TetrisGrid.h"
#include "PaperSprite.h"

//Tetris Grid 
ATetrisGrid::ATetrisGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(RootComponent);
	
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			Tile = CreateDefaultSubobject<UPaperSpriteComponent>(*FString::Printf(TEXT("Tile_x%i_y%i"), i, j));
			Tile->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Tile->SetRelativeLocation(FVector(i * 64.0f, j * 64.0f, 1.0f));
			Tile->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
			Tile->SetSprite(ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Assets/Tiles/TetrisTile")).Object);
		}
	}

}

void ATetrisGrid::BeginPlay()
{
	Super::BeginPlay();

}

void ATetrisGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


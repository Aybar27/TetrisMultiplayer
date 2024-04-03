
#include "TetrisCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Tetris/Figures/TetrisFigures.h"
#include "Tetris/Figures/TetrisFiguresActor.h"
#include "Tetris/PlayerState/TetrisPlayerState.h"
#include "Tetris/GameState/TetrisGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tetris/HUD/TetrisPlayerWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"

//Class to move figures
ATetrisCharacter::ATetrisCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ATetrisCharacter::BeginPlay()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("GAME: Character_Spawned"));

	Super::BeginPlay();

	UWorld* World = GetWorld();

	bool bNotReturn = (GetRemoteRole() == ROLE_Authority || GetRemoteRole() == ROLE_AutonomousProxy) &&
		IsValid(TetrisFigure = Cast<ATetrisFigures>(GetWorld()->SpawnActor(TetrisFigureClass))) &&
		World != nullptr &&
		IsValid(PlayerWidget = CreateWidget<UTetrisPlayerWidget>(GetWorld(), TetrisPlayerWidget));

	if (!bNotReturn) return;

	TetrisFigure->SetGameSpeed(GameSpeed);

	PlayerWidget->AddToViewport();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
}

void ATetrisCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TetrisPlayerState && TetrisGameState) return;

	TetrisPlayerState = Cast<class ATetrisPlayerState>(GetPlayerState());
	TetrisGameState = Cast<class ATetrisGameState>(UGameplayStatics::GetGameState(this));

	bool bNotReturn =
		TetrisPlayerState &&
		TetrisFigure &&
		TetrisGameState &&
		TetrisFigure;

	if (!bNotReturn) return;

	TetrisPlayerState->SetOwnerCharacter(this);
	TetrisFigure->SetPlayerState(TetrisPlayerState);
	TetrisFigure->SetGameState(TetrisGameState);
}

void ATetrisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ATetrisCharacter::MoveDownFast);
	PlayerInputComponent->BindAction("MoveDown", IE_Released, this, &ATetrisCharacter::MoveDownSlow);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ATetrisCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATetrisCharacter::MoveLeft);
	PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ATetrisCharacter::Rotate);
}

void ATetrisCharacter::SetGameSpeed(int32 Speed)
{
	if (!TetrisFigure) return;
	TetrisFigure->SetGameSpeed(GameSpeed / (Speed * ((float)Speed / 2)));
}

void ATetrisCharacter::MoveDownFast()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveDownFast"));

	if (!TetrisFigure) return;
	SpawnedFigure = TetrisFigure->GetSpawnedFigure();

	if (!SpawnedFigure) return;
	SpawnedFigure->ChangeGameSpeed(GameAccelaration);
}

void ATetrisCharacter::MoveDownSlow()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveSlow"));

	if (!TetrisFigure) return;
	SpawnedFigure = TetrisFigure->GetSpawnedFigure();
		
	if (!SpawnedFigure) return;
	SpawnedFigure->ChangeGameSpeed(TetrisFigure->GetGameSpeed());
}

void ATetrisCharacter::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveRight"));

	if (!TetrisFigure) return;
	SpawnedFigure = TetrisFigure->GetSpawnedFigure();

	if (!SpawnedFigure) return;
	SpawnedFigure->MoveRight();
}

void ATetrisCharacter::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveLeft"));

	if (!TetrisFigure) return;
	SpawnedFigure = TetrisFigure->GetSpawnedFigure();

	if (!SpawnedFigure) return;
	SpawnedFigure->MoveLeft();
}

void ATetrisCharacter::Rotate()
{
	UE_LOG(LogTemp, Warning, TEXT("Rotate"));

	if (!TetrisFigure) return;
	SpawnedFigure = TetrisFigure->GetSpawnedFigure();

	if (!SpawnedFigure) return;
	SpawnedFigure->RotateFigure();
}

void ATetrisCharacter::SpawnBlocks()
{
	if (!TetrisFigure) return;
	TetrisFigure->SetIsSpawnBlocks();
}

void ATetrisCharacter::SetGameIsOver()
{
	if (!PlayerWidget) return;
	PlayerWidget->SetWinningText(false);
}

void ATetrisCharacter::SetGameIsWinning()
{
	if (!PlayerWidget) return;
	PlayerWidget->SetWinningText(true);
}

void ATetrisCharacter::SetPlayerPoints(int32 Points)
{
	if (!PlayerWidget) return;
	PlayerWidget->SetPlayerPoints(Points);
}

void ATetrisCharacter::SetOpponentPoints(int32 Points)
{
	if (!PlayerWidget) return;
	PlayerWidget->SetOpponentPoints(Points);
}

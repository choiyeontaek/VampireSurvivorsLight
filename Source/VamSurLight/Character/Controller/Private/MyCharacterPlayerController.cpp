// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterPlayerController.h"
#include "InputMappingContext.h" /*UInputMappingContext*/
#include "EnhancedInputSubsystems.h" /*UEnhancedInputLocalPlayerSubsystem*/
#include "EnhancedInputComponent.h" /*UEnhancedInputComponent*/
#include "Kismet/KismetMathLibrary.h" /*getRightVector*/

AMyCharacterPlayerController::AMyCharacterPlayerController()
{
	// input mapping context load
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ImcPlayer
		(TEXT("/Game/player/input/imc_player.imc_player"));
	if (ImcPlayer.Succeeded()) {
		InputMappingContext = ImcPlayer.Object;
	}

	// input action load
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMove
		(TEXT("/Game/player/input/ia_move.ia_move"));
	if (InputActionMove.Succeeded()) {
		MoveAction = InputActionMove.Object;
	}
}

void AMyCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())}) {
		subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AMyCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent{CastChecked<UEnhancedInputComponent>(InputComponent)}) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayerController::MovePlayer);
	}
}

// character movement
void AMyCharacterPlayerController::MovePlayer(const FInputActionValue& Value)
{
	const FVector2D MovementVector{Value.Get<FVector2D>()};
	if (APawn* ControlledPawn{GetPawn()}) {
		FVector MoveXAxis{UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, 0.f))};
		FVector MoveYAxis{UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, GetControlRotation().Roll))};
		ControlledPawn->AddMovementInput(MoveXAxis, MovementVector.X);
		ControlledPawn->AddMovementInput(MoveYAxis, MovementVector.Y);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterPlayerController.h"
#include "InputMappingContext.h" /*UInputMappingContext*/
#include "EnhancedInputSubsystems.h" /*UEnhancedInputLocalPlayerSubsystem*/
#include "EnhancedInputComponent.h" /*UEnhancedInputComponent*/
#include "Kismet/KismetMathLibrary.h" /*getRightVector*/

AMyCharacterPlayerController::AMyCharacterPlayerController()
{
	// input mapping context load
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imcPlayer
		(TEXT("/Game/player/input/imc_player.imc_player"));
	if (imcPlayer.Succeeded()) {
		inputMappingContext = imcPlayer.Object;
	}

	// input action load
	static ConstructorHelpers::FObjectFinder<UInputAction> inputActionMove
		(TEXT("/Game/player/input/ia_move.ia_move"));
	if (inputActionMove.Succeeded()) {
		moveAction = inputActionMove.Object;
	}
}

void AMyCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())}) {
		subsystem->AddMappingContext(inputMappingContext, 0);
	}
}

void AMyCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* enhancedInputComponent{CastChecked<UEnhancedInputComponent>(InputComponent)}) {
		enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayerController::MovePlayer);
	}
}

// character movement
void AMyCharacterPlayerController::MovePlayer(const FInputActionValue& value)
{
	const FVector2D movementVector{value.Get<FVector2D>()};
	if (APawn* ControlledPawn{GetPawn()}) {
		FVector moveXAxis{UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, 0.f))};
		FVector moveYAxis{UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, GetControlRotation().Roll))};
		ControlledPawn->AddMovementInput(moveXAxis, movementVector.X);
		ControlledPawn->AddMovementInput(moveYAxis, movementVector.Y);
	}
}

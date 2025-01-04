// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AMyCharacterPlayerController::AMyCharacterPlayerController() {
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> imcPlayer
    (TEXT("/Script/EnhancedInput.InputMappingContext'/Game/player/input/imc_player.imc_player'"));
    if (imcPlayer.Succeeded()) {
        inputMappingContext = imcPlayer.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> inputActionMove
    (TEXT("/Script/EnhancedInput.InputAction'/Game/player/input/ia_move.ia_move'"));
    if (inputActionMove.Succeeded()) {
        moveAction = inputActionMove.Object;
    }
}

void AMyCharacterPlayerController::BeginPlay() {
	Super::BeginPlay();
    if (UEnhancedInputLocalPlayerSubsystem *subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
        subsystem->AddMappingContext(inputMappingContext, 0);
    }
}

void AMyCharacterPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
    if (UEnhancedInputComponent *enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayerController::MovePlayer);
    }
}

void AMyCharacterPlayerController::MovePlayer(const FInputActionValue &value) {
    const FVector2D movementVector = value.Get<FVector2D>();
    FRotator3f controlRotation{GetControlRotation()};
    if (APawn *ControlledPawn = GetPawn()) {
        ControlledPawn->AddMovementInput(FVector(, 0.f,), movementVector.X); 
    }
}

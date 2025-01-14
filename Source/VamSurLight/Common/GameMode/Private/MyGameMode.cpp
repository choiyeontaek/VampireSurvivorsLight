// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyCharacterPlayerController.h"	/*player controller*/
#include "MyCharacter.h"	/*character*/
#include "SynergyManager.h"

AMyGameMode::AMyGameMode()
{
	PlayerControllerClass = AMyCharacterPlayerController::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLocation{FVector::ZeroVector};
	FRotator SpawnRotation{FRotator::ZeroRotator};
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SynergyManager = GetWorld()->SpawnActor<ASynergyManager>(ASynergyManager::StaticClass(), SpawnLocation,
	                                                         SpawnRotation, SpawnParams);
	// show mouse cursor
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

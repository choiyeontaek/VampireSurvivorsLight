// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyCharacterPlayerController.h"	/*player controller*/
#include "MyCharacter.h"	/*character*/

AMyGameMode::AMyGameMode()
{
	PlayerControllerClass = AMyCharacterPlayerController::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
	
}
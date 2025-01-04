// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacterPlayerController.h"	/*player controller*/
#include "MyCharacter.h"	/*character*/

AMyGameModeBase::AMyGameModeBase()
{
	PlayerControllerClass = AMyCharacterPlayerController::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
}

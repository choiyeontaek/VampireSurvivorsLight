// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API AMyCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyCharacterPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext *inputMappingContext;
};

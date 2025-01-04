// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h" /*inputActionValue*/
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
    void MovePlayer(const FInputActionValue &value);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* moveAction;
};

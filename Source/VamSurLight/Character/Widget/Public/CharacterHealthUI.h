// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHealthUI.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UCharacterHealthUI : public UUserWidget {
	GENERATED_BODY()
public:
	UCharacterHealthUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UPROPERTY()
	class AMyCharacter* Character;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* CharacterHealthBar;
	UFUNCTION()
	float GetHealthPercentage() const;
	void UpdateHealthBar();
};

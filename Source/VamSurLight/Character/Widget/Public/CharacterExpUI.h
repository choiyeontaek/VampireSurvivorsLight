// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterExpUI.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UCharacterExpUI : public UUserWidget {
	GENERATED_BODY()
public:
	UCharacterExpUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UPROPERTY()
	class AMyCharacter* Character;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* CharacterExpBar;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* LevelText;

	int32 Level;
	
	UFUNCTION()
	float GetExpPercentage() const;
	void UpdateExpBar();
	void UpdateLevelText();
};

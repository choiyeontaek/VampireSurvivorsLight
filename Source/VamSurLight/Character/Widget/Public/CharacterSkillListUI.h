// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillListUI.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UCharacterSkillListUI : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* WeaponOneImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* WeaponOneText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* WeaponTwoImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* WeaponTwoText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* WeaponThreeImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* WeaponThreeText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* StatusOneImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* StatusOneText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* StatusTwoImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* StatusTwoText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* StatusThreeImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* StatusThreeText;
};

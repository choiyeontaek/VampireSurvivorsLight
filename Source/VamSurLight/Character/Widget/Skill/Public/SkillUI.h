// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillUI.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USkillUI : public UUserWidget {
	GENERATED_BODY()
	
public:
	USkillUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* SkillImage;
};

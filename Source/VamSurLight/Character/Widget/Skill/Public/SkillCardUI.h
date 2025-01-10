// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCardUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClicked, int32, CardIndex);
/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USkillCardUI : public UUserWidget {
	GENERATED_BODY()
public:
	USkillCardUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* ChooseButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillUI* widget_skill;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* skillText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardIndex;

	UPROPERTY(BlueprintAssignable)
	FOnCardClicked OnCardClicked;

private:
	UFUNCTION()
	void OnButtonClicked();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SynergyManager.h"
#include "SkillChooseUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillSelected, EWeaponType, SelectedWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusSelected, EStatusType, SelectedStatus);
/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USkillChooseUI : public UUserWidget {
	GENERATED_BODY()
public:
	USkillChooseUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* widget_skillCard_one;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* widget_skillCard_two;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* widget_skillCard_three;
	
	UFUNCTION(BlueprintCallable)
	void SetupSkillCards(EWeaponType Weapon1, EWeaponType Weapon2, EWeaponType Weapon3, EStatusType Status);

	UPROPERTY(BlueprintAssignable)
	FOnSkillSelected OnWeaponSelected;

	UPROPERTY(BlueprintAssignable)
	FOnStatusSelected OnStatusSelected;

private:
	UFUNCTION()
	void OnSkillCardOneClicked() { OnSkillCardClicked(0); }

	UFUNCTION()
	void OnSkillCardTwoClicked() { OnSkillCardClicked(1); }

	UFUNCTION()
	void OnSkillCardThreeClicked() { OnSkillCardClicked(2); }
	
	UFUNCTION()
	void OnSkillCardClicked(int32 CardIndex);

	TArray<EWeaponType> WeaponOptions;
	EStatusType StatusOption;
};

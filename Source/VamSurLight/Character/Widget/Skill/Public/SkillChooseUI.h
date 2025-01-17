#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SynergyManager.h"
#include "SkillChooseUI.generated.h"

USTRUCT(BlueprintType)
struct FCardOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusType StatusType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHealth;
	
	FCardOption() : WeaponType(EWeaponType::None), StatusType(EStatusType::None), bIsWeapon(true), bIsHealth(false)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, FCardOption, SelectedOption);

UCLASS()
class VAMSURLIGHT_API USkillChooseUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* SkillCard1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* SkillCard2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USkillCardUI* SkillCard3;

	UFUNCTION(BlueprintCallable)
	void SetupCards();

	UPROPERTY(BlueprintAssignable)
	FOnOptionSelected OnOptionSelected;

	UPROPERTY(BlueprintReadOnly)
	class ALevelUpManager* LevelUpManager;

	UPROPERTY()
	class ASynergyManager* SynergyManager;

private:
	UFUNCTION()
	void OnCardClicked(int32 CardIndex);

	TArray<FCardOption> CardOptions;
};

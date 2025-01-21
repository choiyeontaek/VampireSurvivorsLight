#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillChooseUI.h"
#include "SkillCardUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClicked, int32, CardIndex);

UCLASS()
class VAMSURLIGHT_API USkillCardUI : public UUserWidget
{
	GENERATED_BODY()

public:
	USkillCardUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* ChooseButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* CardImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* CardText;

	UFUNCTION(BlueprintCallable)
	void SetupCard(const FCardOption& Option, int32 Index);

	UPROPERTY(BlueprintAssignable)
	FOnCardClicked OnCardClicked;

	// asset data
	UPROPERTY(EditDefaultsOnly, Category = "Card Data")
	class UCardDataAsset* CardData;

	UPROPERTY(BlueprintReadOnly)
	class ALevelUpManager* LevelUpManager;


private:
	UFUNCTION()
	void OnButtonClicked();

	int32 CardIndex;
};
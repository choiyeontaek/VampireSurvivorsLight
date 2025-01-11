#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillChooseUI.h"
#include "LevelUpManager.generated.h"

UCLASS()
class VAMSURLIGHT_API ULevelUpManager : public UObject
{
	GENERATED_BODY()

public:
	void HandleLevelUp(class AMyCharacter* Character);

private:
	UPROPERTY()
	class AMyCharacter* CurrentCharacter;

	UPROPERTY()
	class USkillChooseUI* SkillChooseWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SkillChooseWidgetClass;

	UFUNCTION()
	void OnOptionSelected(FCardOption SelectedOption);

	void ShowSkillChooseUI();
};

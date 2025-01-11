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
	ULevelUpManager(const FObjectInitializer& ObjectInitializer);;
	void HandleLevelUp(class AMyCharacter* Character);

private:
	UPROPERTY()
	class AMyCharacter* CurrentCharacter;

	UPROPERTY()
	class USkillChooseUI* SkillChooseWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillChooseUI> SkillChooseWidgetClass;

	UFUNCTION()
	void OnOptionSelected(FCardOption SelectedOption);

	void ShowSkillChooseUI();

public:
	int32 AutoAttackLevel{1};
	int32 BoomerangLevel{1};
	int32 TrainLevel{1};
	int32 ForceFieldLevel{1};
	int32 GuardianLevel{1};

	int32 CoolTimeLevel{1};
	int32 MovementSpeedLevel{1};
	int32 DamageLevel{1};
	int32 MaxHealthLevel{1};
	int32 HealthRegenerationLevel{1};
	
};

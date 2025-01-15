#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "SkillChooseUI.h"
#include "LevelUpManager.generated.h"

UCLASS()
class VAMSURLIGHT_API ALevelUpManager : public AActor
{
	GENERATED_BODY()

public:
	ALevelUpManager(const FObjectInitializer& ObjectInitializer);;
	virtual void BeginPlay() override;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 AutoAttackLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BoomerangLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TrainLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ForceFieldLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GuardianLevel{1};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CoolTimeLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MovementSpeedLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 DamageLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxHealthLevel{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthRegenerationLevel{1};
};

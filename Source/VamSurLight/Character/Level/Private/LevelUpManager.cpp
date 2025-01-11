#include "LevelUpManager.h"
#include "SkillChooseUI.h"
#include "MyCharacter.h"
#include "LogUtils.h"
#include "Kismet/GameplayStatics.h"

ULevelUpManager::ULevelUpManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<USkillChooseUI> WidgetClassFinder
		(TEXT("/Game/player/widget/skill/widget_skillChoose.widget_skillChoose_C"));
	if (WidgetClassFinder.Succeeded()) {
		SkillChooseWidgetClass = WidgetClassFinder.Class;
	}
}

void ULevelUpManager::HandleLevelUp(AMyCharacter* Character)
{
	CurrentCharacter = Character;
	ShowSkillChooseUI();
}

void ULevelUpManager::ShowSkillChooseUI()
{
	if (SkillChooseWidgetClass) {
		SkillChooseWidget = CreateWidget<USkillChooseUI>(GetWorld(), SkillChooseWidgetClass);
		if (SkillChooseWidget) {
			SkillChooseWidget->OnOptionSelected.AddDynamic(this, &ULevelUpManager::OnOptionSelected);
			SkillChooseWidget->SetupCards();
			SkillChooseWidget->AddToViewport();

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController) {
				PlayerController->SetPause(true);
				PlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void ULevelUpManager::OnOptionSelected(FCardOption SelectedOption)
{
	if (SelectedOption.bIsWeapon) {
		LogUtils::Log("Selected Weapon", static_cast<int32>(SelectedOption.WeaponType));
		switch (SelectedOption.WeaponType) {
		case EWeaponType::None:
			break;
		case EWeaponType::AutoAttack:
			++AutoAttackLevel;
			break;
		case EWeaponType::Boomerang:
			++BoomerangLevel;
			break;
		case EWeaponType::Train:
			++TrainLevel;
			break;
		case EWeaponType::ForceField:
			++ForceFieldLevel;
			break;
		case EWeaponType::Guardian:
			++GuardianLevel;
			break;
		case EWeaponType::MAX:
			break;
		}
	}
	else {
		LogUtils::Log("Selected Status", static_cast<int32>(SelectedOption.StatusType));
		switch (SelectedOption.StatusType) {
		case EStatusType::None:
			break;
		case EStatusType::CoolTimeUpdate:
			++CoolTimeLevel;
			break;
		case EStatusType::MovementSpeedUpdate:
			++MovementSpeedLevel;
			break;
		case EStatusType::DamageUpdate:
			++DamageLevel;
			break;
		case EStatusType::MaxHealthUpdate:
			++MaxHealthLevel;
			break;
		case EStatusType::HealthRegenerationUpdate:
			++HealthRegenerationLevel;
			break;
		case EStatusType::MAX:
			break;
		}
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) {
		PlayerController->SetPause(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SkillChooseWidget->RemoveFromParent();
}

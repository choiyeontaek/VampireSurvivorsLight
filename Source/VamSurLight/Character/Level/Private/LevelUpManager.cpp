#include "LevelUpManager.h"

#include "SkillChooseUI.h"
#include "MyCharacter.h"
#include "LogUtils.h"
#include "Kismet/GameplayStatics.h"

ALevelUpManager::ALevelUpManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<USkillChooseUI> WidgetClassFinder
		(TEXT("/Game/player/widget/skill/widget_skillChoose.widget_skillChoose_C"));
	if (WidgetClassFinder.Succeeded()) {
		SkillChooseWidgetClass = WidgetClassFinder.Class;
	}
}

void ALevelUpManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelUpManager::HandleLevelUp(AMyCharacter* Character)
{
	CurrentCharacter = Character;
	ShowSkillChooseUI();
}

int32 ALevelUpManager::GetWeaponLevel(EWeaponType Weapon)
{
	switch (Weapon) {
	case EWeaponType::AutoAttack:
		return AutoAttackLevel;
	case EWeaponType::Boomerang:
		return BoomerangLevel;
	case EWeaponType::Train:
		return TrainLevel;
	case EWeaponType::ForceField:
		return ForceFieldLevel;
	case EWeaponType::Guardian:
		return GuardianLevel;
	default:
		return 0;
	}
}

int32 ALevelUpManager::GetStatusLevel(EStatusType Status)
{
	switch (Status) {
	case EStatusType::CoolTimeUpdate:
		return CoolTimeLevel;
	case EStatusType::MovementSpeedUpdate:
		return MovementSpeedLevel;
	case EStatusType::DamageUpdate:
		return DamageLevel;
	case EStatusType::MaxHealthUpdate:
		return MaxHealthLevel;
	case EStatusType::HealthRegenerationUpdate:
		return HealthRegenerationLevel;
	default:
		return 0;
	}
}

void ALevelUpManager::ShowSkillChooseUI()
{
	if (SkillChooseWidgetClass) {
		SkillChooseWidget = CreateWidget<USkillChooseUI>(GetWorld(), SkillChooseWidgetClass);
		if (SkillChooseWidget) {
			SkillChooseWidget->OnOptionSelected.AddDynamic(this, &ALevelUpManager::OnOptionSelected);
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

void ALevelUpManager::OnOptionSelected(FCardOption SelectedOption)
{
	if (SelectedOption.bIsHealth) {
		CurrentCharacter->AddHealth(10.f);
	}
	else if (SelectedOption.bIsWeapon) {
		LogUtils::Log("Selected Weapon", static_cast<int32>(SelectedOption.WeaponType));
		CurrentCharacter->UpdateSkillUI(SelectedOption, GetWeaponLevel(SelectedOption.WeaponType) + 1);
		switch (SelectedOption.WeaponType) {
		case EWeaponType::None:
			break;
		case EWeaponType::AutoAttack:
			++AutoAttackLevel;
			break;
		case EWeaponType::Boomerang:
			++BoomerangLevel;
			if (1 == BoomerangLevel) {
				CurrentCharacter->StartAttack(EWeaponType::Boomerang);
			}
			break;
		case EWeaponType::Train:
			++TrainLevel;
			if (1 == TrainLevel) {
				CurrentCharacter->StartAttack(EWeaponType::Train);
			}
			break;
		case EWeaponType::ForceField:
			++ForceFieldLevel;
			if (1 == ForceFieldLevel) {
				CurrentCharacter->StartAttack(EWeaponType::ForceField);
			}
			break;
		case EWeaponType::Guardian:
			++GuardianLevel;
			if (1 == GuardianLevel) {
				CurrentCharacter->StartAttack(EWeaponType::Guardian);
			}
			break;
		case EWeaponType::MAX:
			break;
		}
	}
	else {
		LogUtils::Log("Selected Status", static_cast<int32>(SelectedOption.StatusType));
		CurrentCharacter->UpdateSkillUI(SelectedOption, GetStatusLevel(SelectedOption.StatusType) + 1);
		switch (SelectedOption.StatusType) {
		case EStatusType::None:
			break;
		case EStatusType::CoolTimeUpdate:
			++CoolTimeLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::MovementSpeedUpdate:
			++MovementSpeedLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::DamageUpdate:
			++DamageLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::MaxHealthUpdate:
			++MaxHealthLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::HealthRegenerationUpdate:
			++HealthRegenerationLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
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

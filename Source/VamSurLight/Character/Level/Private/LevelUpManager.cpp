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
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::MovementSpeedUpdate:
			++MovementSpeedLevel;
			CurrentCharacter->StatusLevelUp(SelectedOption.StatusType);
			break;
		case EStatusType::DamageUpdate:
			++DamageLevel;
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

	LogUtils::Log("Level", AutoAttackLevel);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) {
		PlayerController->SetPause(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SkillChooseWidget->RemoveFromParent();
}

#include "LevelUpManager.h"

#include "AutoAttackWeapon.h"
#include "BoomerangWeapon.h"
#include "ForceFieldWeapon.h"
#include "GuardianWeapon.h"
#include "SkillChooseUI.h"
#include "MyCharacter.h"
#include "LogUtils.h"
#include "SkillAutoAttack.h"
#include "SkillBoomerang.h"
#include "SkillForceField.h"
#include "SkillGuardian.h"
#include "SkillTrain.h"
#include "TrainWeapon.h"
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

	// get weapons
	AutoAttackWeapon = Cast<AAutoAttackWeapon>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AAutoAttackWeapon::StaticClass()));
	BoomerangWeapon = Cast<ABoomerangWeapon>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ABoomerangWeapon::StaticClass()));
	TrainWeapon = Cast<ATrainWeapon>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrainWeapon::StaticClass()));
	ForceFieldWeapon = Cast<AForceFieldWeapon>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AForceFieldWeapon::StaticClass()));
	GuardianWeapon = Cast<AGuardianWeapon>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGuardianWeapon::StaticClass()));

	SkillAutoAttack = Cast<ASkillAutoAttack>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASkillAutoAttack::StaticClass()));
	SkillBoomerang = Cast<ASkillBoomerang>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASkillBoomerang::StaticClass()));
	SkillTrain = Cast<ASkillTrain>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkillTrain::StaticClass()));
	SkillForceField = Cast<ASkillForceField>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASkillForceField::StaticClass()));
	SkillGuardian = Cast<ASkillGuardian>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkillGuardian::StaticClass()));
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
			AutoAttackWeapon->LevelUp();
			SkillAutoAttack->LevelUp();
			break;
		case EWeaponType::Boomerang:
			++BoomerangLevel;
			BoomerangWeapon->LevelUp();
			SkillBoomerang->LevelUp();
			break;
		case EWeaponType::Train:
			++TrainLevel;
			TrainWeapon->LevelUp();
			SkillTrain->LevelUp();
			break;
		case EWeaponType::ForceField:
			++ForceFieldLevel;
			ForceFieldWeapon->LevelUp();
			SkillForceField->LevelUp();
			break;
		case EWeaponType::Guardian:
			++GuardianLevel;
			GuardianWeapon->LevelUp();
			SkillGuardian->LevelUp();
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
			AutoAttackWeapon->DamageLevelUp();
			BoomerangWeapon->DamageLevelUp();
			TrainWeapon->DamageLevelUp();
			ForceFieldWeapon->DamageLevelUp();
			GuardianWeapon->DamageLevelUp();
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

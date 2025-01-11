#include "LevelUpManager.h"
#include "SkillChooseUI.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

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
				//PlayerController->bShowMouseCursor = true;
			}
		}
	}
}

void ULevelUpManager::OnOptionSelected(FCardOption SelectedOption)
{
	if (SelectedOption.bIsWeapon) {
		UE_LOG(LogTemp, Warning, TEXT("Selected Weapon: %s"), *UEnum::GetValueAsString(SelectedOption.WeaponType));
		// 여기에 무기 선택 처리 로직 추가
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected Status: %s"), *UEnum::GetValueAsString(SelectedOption.StatusType));
		// 여기에 상태 선택 처리 로직 추가
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) {
		PlayerController->SetPause(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		//PlayerController->bShowMouseCursor = false;
	}

	SkillChooseWidget->RemoveFromParent();
}

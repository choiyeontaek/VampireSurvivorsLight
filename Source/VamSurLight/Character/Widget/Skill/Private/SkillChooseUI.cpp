// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillChooseUI.h"
#include "SkillCardUI.h"
#include "Components/Button.h"

USkillChooseUI::USkillChooseUI(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{}

void USkillChooseUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (widget_skillCard_one && widget_skillCard_one->ChooseButton)
		widget_skillCard_one->ChooseButton->OnClicked.AddDynamic(this, &USkillChooseUI::OnSkillCardOneClicked);
	if (widget_skillCard_two && widget_skillCard_two->ChooseButton)
		widget_skillCard_two->ChooseButton->OnClicked.AddDynamic(this, &USkillChooseUI::OnSkillCardTwoClicked);
	if (widget_skillCard_three && widget_skillCard_three->ChooseButton)
		widget_skillCard_three->ChooseButton->OnClicked.AddDynamic(this, &USkillChooseUI::OnSkillCardThreeClicked);
}

void USkillChooseUI::SetupSkillCards(EWeaponType Weapon1, EWeaponType Weapon2, EWeaponType Weapon3, EStatusType Status)
{
	WeaponOptions = {Weapon1, Weapon2, Weapon3};
	StatusOption = Status;

	// Setup the skill cards here (set images and text)
	// You'll need to implement this part based on your CardDataAsset
}

void USkillChooseUI::OnSkillCardClicked(int32 CardIndex)
{
	if (CardIndex >= 0 && CardIndex < 3)
	{
		OnWeaponSelected.Broadcast(WeaponOptions[CardIndex]);
	}
	else if (CardIndex == 3)
	{
		OnStatusSelected.Broadcast(StatusOption);
	}

	RemoveFromParent();
}


/*
* void AYourGameMode::ShowSkillChooseUI()
{
	USkillChoose* SkillChooseWidget = CreateWidget<USkillChoose>(GetWorld(), SkillChooseWidgetClass);
	if (SkillChooseWidget)
	{
		SkillChooseWidget->SetupSkillCards(EWeaponType::AutoAttack, EWeaponType::Train, EWeaponType::Boomerang, EStatusType::HealthRegenerationUpdate);
		SkillChooseWidget->OnWeaponSelected.AddDynamic(this, &AYourGameMode::OnWeaponSelected);
		SkillChooseWidget->OnStatusSelected.AddDynamic(this, &AYourGameMode::OnStatusSelected);
		SkillChooseWidget->AddToViewport();
	}
}

void AYourGameMode::OnWeaponSelected(EWeaponType SelectedWeapon)
{
	// Handle the selected weapon
	UE_LOG(LogTemp, Warning, TEXT("Selected Weapon: %s"), *UEnum::GetValueAsString(SelectedWeapon));
}

void AYourGameMode::OnStatusSelected(EStatusType SelectedStatus)
{
	// Handle the selected status
	UE_LOG(LogTemp, Warning, TEXT("Selected Status: %s"), *UEnum::GetValueAsString(SelectedStatus));
}
 */
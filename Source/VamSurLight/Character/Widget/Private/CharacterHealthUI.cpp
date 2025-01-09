// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHealthUI.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/
#include "Components/ProgressBar.h" /*progress bar*/
#include "Kismet/GameplayStatics.h" /*get player character*/

UCharacterHealthUI::UCharacterHealthUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UCharacterHealthUI::NativeConstruct() {
	Super::NativeConstruct();

	Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CharacterHealthBar && Character) {
		CharacterHealthBar->PercentDelegate.BindUFunction(this, FName("GetHealthPercentage"));
		UpdateHealthBar();
	}
}

float UCharacterHealthUI::GetHealthPercentage() const
{
	if (Character) {
		//LogUtils::Log("GetHealthPercentage", Character->Health, Character->MaxHealth);
		return Character->Health / Character->MaxHealth;
	}
	
	return 0.f;
}

void UCharacterHealthUI::UpdateHealthBar()
{
	if (CharacterHealthBar && Character) {
		CharacterHealthBar->SetPercent(GetHealthPercentage());
	}
}


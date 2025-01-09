// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterExpUI.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/
#include "Components/ProgressBar.h" /*progress bar*/
#include "Components/TextBlock.h" /*Text Block*/
#include "Kismet/GameplayStatics.h" /*get player character*/

UCharacterExpUI::UCharacterExpUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UCharacterExpUI::NativeConstruct() {
	Super::NativeConstruct();
	
	Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CharacterExpBar && Character) {
		CharacterExpBar->PercentDelegate.BindUFunction(this, FName("GetExpPercentage"));
		UpdateExpBar();
	}

	Level = 0;
	UpdateLevelText();
}

float UCharacterExpUI::GetExpPercentage() const
{
	if (Character) {
		//LogUtils::Log("GetExpPercentage", Character->Exp, Character->MaxExp);
		return Character->Exp / Character->MaxExp;
	}
	
	return 0.f;
}
void UCharacterExpUI::UpdateExpBar()
{
	if (CharacterExpBar && Character) {
		CharacterExpBar->SetPercent(GetExpPercentage());
	}
}
void UCharacterExpUI::UpdateLevelText()
{
	++Level;
	if (LevelText) {
		LevelText->SetText(FText::AsNumber(Level));
	}
}
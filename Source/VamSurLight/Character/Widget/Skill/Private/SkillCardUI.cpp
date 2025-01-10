// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCardUI.h"
#include "Components/Button.h"

USkillCardUI::USkillCardUI(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{}

void USkillCardUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChooseButton)
	{
		ChooseButton->OnClicked.AddDynamic(this, &USkillCardUI::OnButtonClicked);
	}
}

void USkillCardUI::OnButtonClicked()
{
	OnCardClicked.Broadcast(CardIndex);
}

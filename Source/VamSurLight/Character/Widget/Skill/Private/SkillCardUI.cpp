#include "SkillCardUI.h"
#include "CardDataAsset.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

USkillCardUI::USkillCardUI(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UCardDataAsset> CardDataAsset
		(TEXT("/Game/Data/dataAsset_card.dataAsset_card"));
	if (CardDataAsset.Succeeded()) {
		CardData = CardDataAsset.Object;
	}
}

void USkillCardUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChooseButton) {
		ChooseButton->OnClicked.AddDynamic(this, &USkillCardUI::OnButtonClicked);
	}
}

void USkillCardUI::SetupCard(const FCardOption& Option, int32 Index)
{
	CardIndex = Index;

	if (Option.bIsHealth) {
		if (CardText) {
			CardText->SetText(CardData->ExtraHealthText);
			if (CardImage)
				CardImage->SetBrushFromTexture(CardData->ExtraHealthImage);
		}
		
		return;
	}
	if (Option.bIsWeapon) {
		// Set weapon
		if (CardText) {
			switch (Option.WeaponType) {
			case EWeaponType::AutoAttack:
				CardText->SetText(CardData->AutoAttackText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->AutoAttackImage);
				break;
			case EWeaponType::Boomerang:
				CardText->SetText(CardData->BoomerangText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->BoomerangImage);
				break;
			case EWeaponType::Train:
				CardText->SetText(CardData->TrainText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->TrainImage);
				break;
			case EWeaponType::ForceField:
				CardText->SetText(CardData->ForceFieldText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->ForceFieldImage);
				break;
			case EWeaponType::Guardian:
				CardText->SetText(CardData->GuardianText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->GuardianImage);
				break;
			default:
				CardText->SetText(FText::FromString(TEXT("Unknown Weapon")));
				break;
			}
		}
	}
	else {
		// Set status
		if (CardText) {
			switch (Option.StatusType) {
			case EStatusType::CoolTimeUpdate:
				CardText->SetText(CardData->CoolTimeUpdateText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->CoolTimeUpdateImage);
				break;
			case EStatusType::MovementSpeedUpdate:
				CardText->SetText(CardData->MovementSpeedUpdateText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->MovementSpeedUpdateImage);
				break;
			case EStatusType::DamageUpdate:
				CardText->SetText(CardData->DamageUpdateText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->DamageUpdateImage);
				break;
			case EStatusType::MaxHealthUpdate:
				CardText->SetText(CardData->MaxHealthUpdateText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->MaxHealthUpdateImage);
				break;
			case EStatusType::HealthRegenerationUpdate:
				CardText->SetText(CardData->HealthRegenerationUpdateText);
				if (CardImage)
					CardImage->SetBrushFromTexture(CardData->HealthRegenerationUpdateImage);
				break;
			default:
				CardText->SetText(FText::FromString(TEXT("Unknown Status")));
				break;
			}
		}
	}
}

void USkillCardUI::OnButtonClicked()
{
	OnCardClicked.Broadcast(CardIndex);
}

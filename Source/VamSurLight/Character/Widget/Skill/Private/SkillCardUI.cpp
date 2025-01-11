#include "SkillCardUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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

	if (Option.bIsWeapon) {
		// 무기 카드 설정
		if (CardText) {
			CardText->SetText(FText::FromString(UEnum::GetValueAsString(Option.WeaponType)));
		}
		// 여기에 무기 이미지 설정 로직 추가
	}
	else {
		// 상태 카드 설정
		if (CardText) {
			CardText->SetText(FText::FromString(UEnum::GetValueAsString(Option.StatusType)));
		}
		// 여기에 상태 이미지 설정 로직 추가
	}
}

void USkillCardUI::OnButtonClicked()
{
	OnCardClicked.Broadcast(CardIndex);
}

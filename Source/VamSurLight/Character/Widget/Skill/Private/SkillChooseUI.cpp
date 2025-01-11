#include "SkillChooseUI.h"
#include "SkillCardUI.h"

void USkillChooseUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillCard1) SkillCard1->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
	if (SkillCard2) SkillCard2->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
	if (SkillCard3) SkillCard3->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
}

void USkillChooseUI::SetupCards()
{
	TArray<FCardOption> AllOptions;

	for (int32 i = 0; i < static_cast<int32>(EWeaponType::MAX); ++i) {
		FCardOption Option;
		Option.WeaponType = static_cast<EWeaponType>(i);
		Option.bIsWeapon = true;
		AllOptions.Add(Option);
	}

	for (int32 i = 0; i < static_cast<int32>(EStatusType::MAX); ++i) {
		FCardOption Option;
		Option.StatusType = static_cast<EStatusType>(i);
		Option.bIsWeapon = false;
		AllOptions.Add(Option);
	}

	CardOptions.Empty();
	for (int32 i = 0; i < 3; ++i) {
		if (AllOptions.Num() > 0) {
			int32 RandomIndex = FMath::RandRange(0, AllOptions.Num() - 1);
			CardOptions.Add(AllOptions[RandomIndex]);
			AllOptions.RemoveAt(RandomIndex);
		}
	}

	if (SkillCard1) SkillCard1->SetupCard(CardOptions[0], 0);
	if (SkillCard2) SkillCard2->SetupCard(CardOptions[1], 1);
	if (SkillCard3) SkillCard3->SetupCard(CardOptions[2], 2);
}

void USkillChooseUI::OnCardClicked(int32 CardIndex)
{
	if (CardIndex >= 0 && CardIndex < CardOptions.Num()) {
		OnOptionSelected.Broadcast(CardOptions[CardIndex]);
	}
}

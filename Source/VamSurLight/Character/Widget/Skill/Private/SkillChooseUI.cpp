#include "SkillChooseUI.h"
#include "SynergyManager.h"
#include "LevelUpManager.h"
#include "LogUtils.h"
#include "SkillCardUI.h"
#include "Utils.h"
#include "Kismet/GameplayStatics.h"

void USkillChooseUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillCard1) {
		SkillCard1->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
	}
	if (SkillCard2) {
		SkillCard2->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
	}
	if (SkillCard3) {
		SkillCard3->OnCardClicked.AddDynamic(this, &USkillChooseUI::OnCardClicked);
	}
}

void USkillChooseUI::SetupCards()
{
	TArray<FCardOption> AllOptions;

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);

	//LogUtils::Log("111111111111", SynergyManager->WeaponBitmask);
	if (3 == SynergyManager->GetWeaponCount()) {
		TArray<int32> Weapons{SynergyManager->FindSetWeapons()};
		for (int32 Index : Weapons) {
			if (LevelUpManager->GetWeaponLevel(static_cast<EWeaponType>(Index)) < 4) {
				FCardOption Option;
				Option.WeaponType = static_cast<EWeaponType>(Index);
				Option.bIsWeapon = true;
				AllOptions.Add(Option);
			}
			if (LevelUpManager->GetWeaponLevel(static_cast<EWeaponType>(Index)) == 4 && SynergyManager->CheckSynergy(
				static_cast<EWeaponType>(Index))) {
				FCardOption Option;
				Option.WeaponType = static_cast<EWeaponType>(Index);
				Option.bIsWeapon = true;
				AllOptions.Add(Option);
			}
		}
	}
	else {
		for (int32 i{static_cast<int32>(EWeaponType::AutoAttack)}; i < static_cast<int32>(EWeaponType::MAX); ++i) {
			// stop when its 5

			if (LevelUpManager->GetWeaponLevel(static_cast<EWeaponType>(i)) < 4) {
				FCardOption Option;
				Option.WeaponType = static_cast<EWeaponType>(i);
				Option.bIsWeapon = true;
				AllOptions.Add(Option);
			}
			// check synergy if its 4
			if (LevelUpManager->GetWeaponLevel(static_cast<EWeaponType>(i)) == 4 && SynergyManager->CheckSynergy(
				static_cast<EWeaponType>(i))) {
				FCardOption Option;
				Option.WeaponType = static_cast<EWeaponType>(i);
				Option.bIsWeapon = true;
				AllOptions.Add(Option);
			}
		}
	}

	if (3 == SynergyManager->GetStatusCount()) {
		TArray<int32> Status{SynergyManager->FindSetStatus()};
		for (int32 Index : Status) {
			if (LevelUpManager->GetStatusLevel(static_cast<EStatusType>(Index)) == 5) {
				continue;
			}
			FCardOption Option;
			Option.StatusType = static_cast<EStatusType>(Index);
			Option.bIsWeapon = false;
			AllOptions.Add(Option);
		}
	}
	else {
		for (int32 i{static_cast<int32>(EStatusType::CoolTimeUpdate)}; i < static_cast<int32>(EStatusType::MAX); ++i) {
			// stop when its 5
			if (LevelUpManager->GetStatusLevel(static_cast<EStatusType>(i)) == 5) {
				continue;
			}
			FCardOption Option;
			Option.StatusType = static_cast<EStatusType>(i);
			Option.bIsWeapon = false;
			AllOptions.Add(Option);
		}
	}

	CardOptions.Empty();
	for (int32 i{}; i < 3; ++i) {
		if (AllOptions.Num() > 0) {
			int32 RandomIndex{FMath::RandRange(0, AllOptions.Num() - 1)};
			CardOptions.Add(AllOptions[RandomIndex]);
			AllOptions.RemoveAt(RandomIndex);
		}
		else {
			// add extra	health option, if no more upgrades left
			FCardOption Option;
			Option.bIsHealth = true;
			CardOptions.Add(Option);
		}
	}

	if (SkillCard1) {
		SkillCard1->SetupCard(CardOptions[0], 0);
	}
	if (SkillCard2) {
		SkillCard2->SetupCard(CardOptions[1], 1);
	}
	if (SkillCard3) {
		SkillCard3->SetupCard(CardOptions[2], 2);
	}
}

void USkillChooseUI::OnCardClicked(int32 CardIndex)
{
	if (CardIndex >= 0 && CardIndex < CardOptions.Num()) {
		OnOptionSelected.Broadcast(CardOptions[CardIndex]);
	}
}

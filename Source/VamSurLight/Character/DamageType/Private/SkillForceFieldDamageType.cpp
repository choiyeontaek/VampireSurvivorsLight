// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillForceFieldDamageType.h"
#include "LevelUpManager.h"
#include "LogUtils.h"
#include "MyCharacter.h"

void USkillForceFieldDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                  AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillForceFieldDamageType::ApplyDamageEffect");
	AMyCharacter* Character{Cast<AMyCharacter>(InstigatedBy->GetCharacter())};
		
	if (Character->LevelUpManager->ForceFieldLevel < 5) {
		// 주변 적에게 피해를 입힙니다.
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}
	else if (Character->LevelUpManager->ForceFieldLevel == 5) {
		// 불타는 영역 안에서 사망한 적은 폭발을 일으킵니다
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}

}

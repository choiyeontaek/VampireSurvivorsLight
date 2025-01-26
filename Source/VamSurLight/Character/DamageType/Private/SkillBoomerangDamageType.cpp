// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBoomerangDamageType.h"
#include "LevelUpManager.h"
#include "LogUtils.h"
#include "MyCharacter.h"

void USkillBoomerangDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                 AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillBoomerangDamageType::ApplyDamageEffect");
	
	AMyCharacter* Character{Cast<AMyCharacter>(InstigatedBy->GetCharacter())};
	if (Character->LevelUpManager->BoomerangLevel < 5) {
		//가장 가까운 적에게 되돌아오는 투사체를 발사합니다.
		
	}
	else if (Character->LevelUpManager->BoomerangLevel == 5) {
		// 투사체가 작은 투사체로 폭발해 원뿔 범위에 피해를 입힙니다.
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}

}

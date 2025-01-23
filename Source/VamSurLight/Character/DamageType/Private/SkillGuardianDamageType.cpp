// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGuardianDamageType.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/
#include "LevelUpManager.h"

void USkillGuardianDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);
	
	LogUtils::Log("SkillGuardianDamageType::ApplyDamageEffect");
	
	AMyCharacter* Character{Cast<AMyCharacter>(InstigatedBy->GetCharacter())};
	if (Character->LevelUpManager->GuardianLevel < 5) {
		// 주위를 도는 투사체가 적에게 피해를 입히고 뒤로 밀어냅니다.
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}
	else if (Character->LevelUpManager->GuardianLevel == 5) {
		// 영구적으로 주위를 도는 투사체가 적에게 피해를 입히고 뒤로 밀어냅니다.
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}
}

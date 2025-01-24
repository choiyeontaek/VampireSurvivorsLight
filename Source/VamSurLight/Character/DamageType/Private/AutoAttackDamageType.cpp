// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttackDamageType.h"

#include "LevelUpManager.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/

void UAutoAttackDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                             AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);
	
	LogUtils::Log("AutoAttackDamageType::ApplyDamageEffect");
	
	AMyCharacter* Character{Cast<AMyCharacter>(InstigatedBy->GetCharacter())};
	if (Character->LevelUpManager->AutoAttackLevel < 5) {
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}
	else if (Character->LevelUpManager->AutoAttackLevel == 5) {
		FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	}

};

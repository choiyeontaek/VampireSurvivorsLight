// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttackDamageType.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/

void UAutoAttackDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);
	
	AMyCharacter* Character{Cast<AMyCharacter>(DamagedActor)};
	if (Character) {
		LogUtils::Log("AutoAttackDamageType::ApplyDamageEffect");
	}
};
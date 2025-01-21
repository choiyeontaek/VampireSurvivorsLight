// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTrainDamageType.h"

#include "LogUtils.h"

void USkillTrainDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                             AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillTrainDamageType::ApplyDamageEffect");

}

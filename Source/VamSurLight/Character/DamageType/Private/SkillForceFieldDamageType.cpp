// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillForceFieldDamageType.h"

#include "LogUtils.h"

void USkillForceFieldDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                  AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillForceFieldDamageType::ApplyDamageEffect");

}

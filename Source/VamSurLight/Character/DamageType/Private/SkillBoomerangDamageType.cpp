// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBoomerangDamageType.h"

#include "LogUtils.h"

void USkillBoomerangDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                 AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillBoomerangDamageType::ApplyDamageEffect");

}

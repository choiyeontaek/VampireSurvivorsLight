// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttackDamageType.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/

void UAutoAttackDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                             AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);
	
	LogUtils::Log("AutoAttackDamageType::ApplyDamageEffect");

	FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
	DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
};

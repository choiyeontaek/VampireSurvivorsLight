// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageType.h"
#include "SkillTrainDamageType.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USkillTrainDamageType : public UBaseDamageType {
	GENERATED_BODY()
	
public:
	virtual void ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const override; 
	
	
};
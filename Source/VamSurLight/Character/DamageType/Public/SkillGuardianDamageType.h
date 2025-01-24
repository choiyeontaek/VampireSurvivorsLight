// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageType.h"
#include "SkillGuardianDamageType.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USkillGuardianDamageType : public UBaseDamageType {
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const override; 
	
};

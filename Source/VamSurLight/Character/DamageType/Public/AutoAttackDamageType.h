// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageType.h"
#include "AutoAttackDamageType.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UAutoAttackDamageType : public UBaseDamageType {
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const override; 
};

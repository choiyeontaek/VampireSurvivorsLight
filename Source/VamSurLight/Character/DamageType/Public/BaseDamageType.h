// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BaseDamageType.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UBaseDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void ApplyDamageEffect(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const;
	virtual void ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser) const; 
};

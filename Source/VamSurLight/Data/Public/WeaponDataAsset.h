// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackRange;
};
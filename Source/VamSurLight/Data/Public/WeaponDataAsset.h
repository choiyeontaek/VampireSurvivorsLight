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
	// Auto Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BaseAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BaseAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BaseAttackRange;

	// guardian
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianRange;
};

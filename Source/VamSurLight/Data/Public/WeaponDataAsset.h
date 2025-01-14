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

	// train
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainStartDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainMovingLength;

	// boomerang
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangMovingLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangRange;
};

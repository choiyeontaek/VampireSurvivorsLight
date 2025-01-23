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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> BaseAttackProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BaseAttackCoolTime;

	// guardian
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> GuardianProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GuardianCoolTime;
	
	// train
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainStartDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainMovingLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> TrainCoolTime;
	
	// boomerang
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangMovingLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> BoomerangProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BoomerangCoolTime;
	
	// force field
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ForceFieldDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ForceFieldRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ForceFieldCoolTime;


	// physics
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoAttackForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomerangForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrainForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GuardianForce;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForceFieldForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoAttackTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomerangTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrainTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GuardianTime;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForceFieldTime;
};

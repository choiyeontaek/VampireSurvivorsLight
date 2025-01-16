// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatusDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UStatusDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Critical;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> HealthRegeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> RangeSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GainRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Experience;
};

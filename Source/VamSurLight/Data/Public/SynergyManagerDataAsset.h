// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SynergyManagerDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USynergyManagerDataAsset : public UDataAsset {
	GENERATED_BODY()
public:
	// weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasAutoAttackWeapon;

	// status update
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasCoolTimeUpdate;
};

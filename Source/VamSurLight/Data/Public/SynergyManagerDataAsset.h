// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SynergyManager.h"
#include "SynergyManagerDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API USynergyManagerDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	TArray<EWeaponType> AcquiredWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	TArray<EStatusType> AcquiredStatuses;
};

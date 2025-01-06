// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UDropItemDataAsset : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExpAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExpCollisionRange;
	
};

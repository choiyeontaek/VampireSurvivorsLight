// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterExp;
	

};

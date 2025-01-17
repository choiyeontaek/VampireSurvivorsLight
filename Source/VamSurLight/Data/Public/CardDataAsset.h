// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CardDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VAMSURLIGHT_API UCardDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	// weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AutoAttackImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BoomerangImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* TrainImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ForceFieldImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* GuardianImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AutoAttackText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText BoomerangText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TrainText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ForceFieldText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText GuardianText;

	// status
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CoolTimeUpdateImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MovementSpeedUpdateImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DamageUpdateImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MaxHealthUpdateImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* HealthRegenerationUpdateImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CoolTimeUpdateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MovementSpeedUpdateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DamageUpdateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MaxHealthUpdateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText HealthRegenerationUpdateText;


	// extra health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ExtraHealthImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ExtraHealthText;
};

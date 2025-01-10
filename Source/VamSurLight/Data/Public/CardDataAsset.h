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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AutoAttackImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* GuardianImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* TrainImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ForceFieldImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BoomerangImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AutoAttackText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText GuardianText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TrainText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ForceFieldText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText BoomerangText;

};

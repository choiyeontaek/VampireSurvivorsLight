﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SynergyManager.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	None UMETA(DisplayName = "None"),
	AutoAttack UMETA(DisplayName = "Auto Attack"),
	Boomerang UMETA(DisplayName = "Boomerang"),
	Train UMETA(DisplayName = "Train"),
	Guardian UMETA(DisplayName = "Guardian")
};

UENUM(BlueprintType)
enum class EStatusType : uint8 {
	None UMETA(DisplayName = "None"),
	CoolTimeUpdate UMETA(DisplayName = "Cool Time Update"),
	MovementSpeedUpdate UMETA(DisplayName = "Movement Speed Update"),
	DamageUpdate UMETA(DisplayName = "Damage Update"),
	HealthRegenerationUpdate UMETA(DisplayName = "Health Regeneration Update")
};

UCLASS()
class VAMSURLIGHT_API ASynergyManager : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASynergyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void AcquireWeapon(EWeaponType WeaponType);
	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void AcquireStatus(EStatusType StatusType);
	UFUNCTION(BlueprintCallable, Category = "Synergy")
	bool CheckSynergy(EWeaponType WeaponType, EStatusType StatusType);
	
	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class USynergyManagerDataAsset* SynergyManagerData;

	uint32 WeaponBitmask;
	uint32 StatusBitmask;
};
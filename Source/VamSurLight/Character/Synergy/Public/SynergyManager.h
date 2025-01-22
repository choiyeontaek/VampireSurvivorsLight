// Fill out your copyright notice in the Description page of Project Settings.

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
	ForceField UMETA(DisplayName = "ForceField"),
	Guardian UMETA(DisplayName = "Guardian"),

	MAX UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EStatusType : uint8 {
	None UMETA(DisplayName = "None"),
	CoolTimeUpdate UMETA(DisplayName = "Cool Time Update"),
	MovementSpeedUpdate UMETA(DisplayName = "Movement Speed Update"),
	DamageUpdate UMETA(DisplayName = "Damage Update"),
	MaxHealthUpdate UMETA(DisplayName = "Max Health Update"),
	HealthRegenerationUpdate UMETA(DisplayName = "Health Regeneration Update"),

	MAX UMETA(DisplayName = "MAX")
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
	bool CheckSynergy(EWeaponType WeaponType);
	int32 GetWeaponCount();
	int32 GetStatusCount();
	TArray<int32> FindSetWeapons();
	TArray<int32> FindSetStatus();
	bool CheckWeapon(EWeaponType WeaponType);
	bool CheckStatus(EStatusType StatusType);
	
	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class USynergyManagerDataAsset* SynergyManagerData;

	uint32 WeaponBitmask;
	uint32 StatusBitmask;
};

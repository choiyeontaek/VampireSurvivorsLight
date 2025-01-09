// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SynergyManager.generated.h"

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

	void AcquireWeapons(int32 WeaponID);
	void AcquireStatus(int32 StatusID);
	bool CheckSynergy(int32 WeaponID, int32 StatusID);
	
	// weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasAutoAttackWeapon;

	// status update
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasCoolTimeUpdate;

	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class USynergyManagerDataAsset* SynergyManagerData;
};

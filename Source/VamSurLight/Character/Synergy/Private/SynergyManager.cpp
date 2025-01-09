// Fill out your copyright notice in the Description page of Project Settings.


#include "SynergyManager.h"
#include "SynergyManagerDataAsset.h"

// Sets default values
ASynergyManager::ASynergyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBitmask = 0;
	StatusBitmask = 0;

	// status data asset
	static ConstructorHelpers::FObjectFinder<USynergyManagerDataAsset> SynergyDataAsset
		(TEXT("/Game/Data/dataAsset_synergyManager.dataAsset_synergyManager"));
	if (SynergyDataAsset.Succeeded()) {
		SynergyManagerData = SynergyDataAsset.Object;
	}
}

void ASynergyManager::BeginPlay()
{
	Super::BeginPlay();

	if (SynergyManagerData) {
		for (const auto& Weapon : SynergyManagerData->AcquiredWeapons) {
			AcquireWeapon(Weapon);
		}
		for (const auto& Status : SynergyManagerData->AcquiredStatuses) {
			AcquireStatus(Status);
		}
	}
}

void ASynergyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASynergyManager::AcquireWeapon(EWeaponType WeaponType)
{
	WeaponBitmask |= (1 << static_cast<uint32>(WeaponType));
}

void ASynergyManager::AcquireStatus(EStatusType StatusType)
{
	StatusBitmask |= (1 << static_cast<uint32>(StatusType));
}

bool ASynergyManager::CheckSynergy(EWeaponType WeaponType, EStatusType StatusType)
{
	// after &, all 0 = false, any 1 = true 
	bool bHasWeapon = WeaponBitmask & (1 << static_cast<uint32>(WeaponType));
	bool bHasStatus = StatusBitmask & (1 << static_cast<uint32>(StatusType));
	return bHasWeapon && bHasStatus;
}

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

bool ASynergyManager::CheckSynergy(EWeaponType WeaponType)
{
	// after &, all 0 = false, any 1 = true 
	bool bHasWeapon = WeaponBitmask & (1 << static_cast<uint32>(WeaponType));
	bool bHasStatus = StatusBitmask & (1 << static_cast<uint32>(WeaponType));
	return bHasWeapon && bHasStatus;
}

// Brian Kernighan's Algorithm
int32 ASynergyManager::GetWeaponCount()
{
	uint32 BitMask{WeaponBitmask};
	int32 Count{};
	while (BitMask) {
		// remove 1 on far right
		// bitmask - 1 : change bits, from 1 on far right to right / ex) 1101000 --> 1100111
		BitMask = BitMask & (BitMask - 1);
		++Count;
	}

	return Count;
}

int32 ASynergyManager::GetStatusCount()
{
	uint32 BitMask{StatusBitmask};
	int32 Count{};
	while (BitMask) {
		BitMask = BitMask & (BitMask - 1);
		++Count;
	}

	return Count;
}

TArray<int32> ASynergyManager::FindSetWeapons()
{
	TArray<int32> Location;
	int32 Index{};
	uint32 BitMask{WeaponBitmask};

	while (BitMask) {
		// check last bit is 1
		if (BitMask & 1) {
			Location.Add(Index);
		}
		// shift for next bit
		BitMask >>= 1;
		++Index;
	}
	
	return Location;
}

TArray<int32> ASynergyManager::FindSetStatus()
{
	TArray<int32> Location;
	int32 Index{};
	uint32 BitMask{StatusBitmask};

	while (BitMask) {
		if (BitMask & 1) {
			Location.Add(Index);
		}
		BitMask >>= 1;
		++Index;
	}
	
	return Location;
}

bool ASynergyManager::CheckWeapon(EWeaponType WeaponType)
{
	return WeaponBitmask & (1 << static_cast<uint32>(WeaponType));
}

bool ASynergyManager::CheckStatus(EStatusType StatusType)
{
	return StatusBitmask & (1 << static_cast<uint32>(StatusType));
}

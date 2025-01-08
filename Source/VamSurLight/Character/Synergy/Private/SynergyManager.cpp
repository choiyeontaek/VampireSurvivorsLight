// Fill out your copyright notice in the Description page of Project Settings.


#include "SynergyManager.h"
#include "SynergyManagerDataAsset.h"

// Sets default values
ASynergyManager::ASynergyManager() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// status data asset
	static ConstructorHelpers::FObjectFinder<USynergyManagerDataAsset> SynergyDataAsset
		(TEXT("/Game/Data/dataAsset_synergyManager.dataAsset_synergyManager"));
	if (SynergyDataAsset.Succeeded()) {
		SynergyManagerData = SynergyDataAsset.Object;
	}
}

// Called when the game starts or when spawned
void ASynergyManager::BeginPlay() {
	Super::BeginPlay();

	if (SynergyManagerData) {
		bHasAutoAttackWeapon = SynergyManagerData->bHasAutoAttackWeapon;
		bHasCoolTimeUpdate = SynergyManagerData->bHasCoolTimeUpdate;
	}
}

// Called every frame
void ASynergyManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASynergyManager::AcquireWeapons(int32 WeaponID)
{
	switch (WeaponID)
	{
	case 0:
		bHasAutoAttackWeapon = true;
		break;
	default:
		break;
	}
}

void ASynergyManager::AcquireStatus(int32 StatusID)
{
	switch (StatusID)
	{
	case 0:
		bHasCoolTimeUpdate = true;
		break;
	default:
		break;
	}
}

bool ASynergyManager::CheckSynergy(int32 WeaponID, int32 StatusID)
{
	bool Weapon{false};
	bool Status{false};

	switch (WeaponID)
	{
	case 0:
		Weapon = bHasAutoAttackWeapon;
		break;
	default:
		break;
	}
	
	switch (StatusID)
	{
	case 0:
		Status = bHasCoolTimeUpdate;
		break;
	default:
		break;
	}
	
	return (Weapon && Status);
}


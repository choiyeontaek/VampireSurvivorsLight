// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBoomerang.h"
#include "WeaponDataAsset.h"
#include "BoomerangWeapon.h"
#include "LevelUpManager.h"
#include "StatusDataAsset.h"
#include "SynergyManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" /*GetActorOfClass*/
#include "Kismet/KismetMathLibrary.h" /*FindLookAtRotation*/


class UStatusDataAsset;
// Sets default values
ASkillBoomerang::ASkillBoomerang()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// status data asset
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}
	// set weapon class
	BoomerangWeapon = ABoomerangWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASkillBoomerang::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);
	
	WeaponLevel = LevelUpManager->BoomerangLevel;
	ProjectileLevel = 0;
	// initialize data
	if (StatusData) {
		StatusProjectile = StatusData->Projectile[ProjectileLevel];
		WeaponProjectile = WeaponData->BoomerangProjectile[WeaponLevel];
	}

	Attack(StatusProjectile + WeaponProjectile);
}

// Called every frame
void ASkillBoomerang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkillBoomerang::Attack(int32 Count)
{
	if (BoomerangWeapon && GetWorld()) {
		for (int32 i{}; i < Count; i++) {
			FTimerHandle WaitHandle;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]() {
				FVector SpawnLocation = OwningCharacter->GetActorLocation();
				FRotator SpawnRotation = OwningCharacter->GetActorRotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ABoomerangWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABoomerangWeapon>(
					BoomerangWeapon, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedWeapon) {
					SpawnedWeapon->SetOwner(OwningCharacter);
				}
			}, 0.2f * (i + 1), false);
		}
	}
}

void ASkillBoomerang::DestroyActor()
{
	Destroy();
}

void ASkillBoomerang::LevelUp()
{}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAutoAttack.h"
#include "AutoAttackWeapon.h"
#include "LogUtils.h"	/*log*/
#include "StatusDateAsset.h"
#include "SynergyManager.h"	/*synergyCheck*/
#include "Kismet/GameplayStatics.h"

// Sets default values
ASkillAutoAttack::ASkillAutoAttack() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// status data asset
	static ConstructorHelpers::FObjectFinder<UStatusDateAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}

	// set auto attack weapon class
	AutoAttackWeapon = AAutoAttackWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASkillAutoAttack::BeginPlay() {
	Super::BeginPlay();
	
	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);
	
	if (StatusData) {
		Projectile = StatusData->Projectile;
	}

	Attack(Projectile + 3);
}

// Called every frame
void ASkillAutoAttack::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASkillAutoAttack::Attack(int32 Count) {
	LogUtils::Log("ASkillAutoAttack::AutoAttack");

	if (SynergyManager)
	{
		if (SynergyManager->CheckSynergy(0, 0))
		{
			LogUtils::Log("Yes Synergy");
		}
		else
		{
			LogUtils::Log("No Synergy");
		}
	}
	else
	{
		LogUtils::Log("SynergyManager is not initialized");
	}
	
	if (AutoAttackWeapon && GetWorld()) {
		for (int32 i{}; i < Count; i++) {
			FTimerHandle WaitHandle;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]() {
				FVector SpawnLocation = GetActorLocation();
				FRotator SpawnRotation = GetActorRotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AAutoAttackWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AAutoAttackWeapon>( AutoAttackWeapon, SpawnLocation, SpawnRotation, SpawnParams);
			}, 0.1f * (i + 1), false);
		}
	}
}

void ASkillAutoAttack::DestroyActor() {
	Destroy();
}

﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h"
#include "WeaponDataAsset.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATrainWeapon::ATrainWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// collision
	TrainCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TrainCollision"));
	SetRootComponent(TrainCollision);
	TrainCollision->SetSphereRadius(50.f);
	TrainCollision->SetCollisionProfileName(FName("Weapon"));

	// bullet mesh
	TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
	TrainMesh->SetupAttachment(RootComponent);
	TrainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// bullet Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/baseWeapon/sm_autoAttack.sm_autoAttack"));
	if (StaticMeshAsset.Succeeded()) {
		TrainMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	TrainCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrainWeapon::OnOverlapBegin);
	
	// data asset
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}
	
	LevelUpManager = CreateDefaultSubobject<ULevelUpManager>(TEXT("LevelUpManager"));
}

// Called when the game starts or when spawned
void ATrainWeapon::BeginPlay()
{
	Super::BeginPlay();

	Level = LevelUpManager->AutoAttackLevel;

	// initialize with data asset
	if (WeaponData) {
		TrainDamage = WeaponData->TrainDamage[Level - 1];
		TrainSpeed = WeaponData->TrainSpeed[Level - 1];
		TrainMovingLength = WeaponData->TrainMovingLength[Level - 1];
		TrainStartDistance = WeaponData->TrainStartDistance[Level - 1];
	}
	
	TrainStartDistance = 800.f;
	// make random angle 0 ~ 360
	float RandomAngle{2 * PI * FMath::RandRange(0.f, 1.f)};
	// Polar to Cartesian
	RandomPosition = {TrainStartDistance * FMath::Cos(RandomAngle), TrainStartDistance * (FMath::Sin(RandomAngle)), GetOwner()->GetActorLocation().Z};
	// position by player
	RandomPosition.X += GetOwner()->GetActorLocation().X;
	RandomPosition.Y += GetOwner()->GetActorLocation().Y;
	
	MovingDirection = GetOwner()->GetActorLocation() - RandomPosition;
}

// Called every frame
void ATrainWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + MovingDirection * TrainSpeed * DeltaTime);

	float Distance = FVector::Dist(RandomPosition, GetActorLocation());

	if (Distance > TrainMovingLength) {
		DestroyActor();
	}

}

void ATrainWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log();

		//UGameplayStatics::ApplyDamage(OtherActor, TrainDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());

		Destroy();
	}
}

void ATrainWeapon::DestroyActor()
{	
	Destroy();
}
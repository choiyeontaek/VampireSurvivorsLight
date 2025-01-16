// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceFieldWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h"
#include "WeaponDataAsset.h"
#include "StatusDataAsset.h"
#include "Utils.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AForceFieldWeapon::AForceFieldWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// data asset
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}
	// collision
	ForceFieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ForceFieldCollision"));
	RootComponent = ForceFieldCollision;
	ForceFieldCollision->SetSphereRadius(50.f);
	ForceFieldCollision->SetCollisionProfileName(FName("Weapon"));

	// Guardian mesh
	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
	ForceFieldMesh->SetupAttachment(RootComponent);
	ForceFieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Guardian Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/forceFieldWeapon/sm_forceFieldAttack.sm_forceFieldAttack"));
	if (StaticMeshAsset.Succeeded()) {
		ForceFieldMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	ForceFieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AForceFieldWeapon::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void AForceFieldWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);
	
	Level = LevelUpManager->ForceFieldLevel;
	DamageLevel = LevelUpManager->DamageLevel;
	
	// initialize with data asset
	if (WeaponData) {
		ForceFieldDamage = WeaponData->ForceFieldDamage[Level] * StatusData->Damage[DamageLevel];
		ForceFieldRange = WeaponData->ForceFieldRange[Level];
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AForceFieldWeapon::DestroyActor,
									   1.f, false);
}

// Called every frame
void AForceFieldWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowPlayer();
}

void AForceFieldWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{}

void AForceFieldWeapon::LevelUp()
{
	Level = LevelUpManager->ForceFieldLevel;
	ForceFieldDamage = WeaponData->ForceFieldDamage[Level - 1] * StatusData->Damage[DamageLevel];
}

void AForceFieldWeapon::DamageLevelUp()
{
	DamageLevel = LevelUpManager->DamageLevel;
	ForceFieldDamage = WeaponData->ForceFieldDamage[Level - 1] * StatusData->Damage[DamageLevel];
}

void AForceFieldWeapon::FollowPlayer()
{
	AActor* OwningActor = GetOwner();
	
	if (OwningActor) {
		FVector NewLocation = OwningActor->GetActorLocation();
		NewLocation.Z += 100.0f;
		SetActorLocation(NewLocation);
	}
}

void AForceFieldWeapon::DestroyActor()
{
	Destroy();
}



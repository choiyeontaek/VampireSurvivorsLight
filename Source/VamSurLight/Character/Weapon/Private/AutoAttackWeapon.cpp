// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttackWeapon.h"
#include "AutoAttackDamageType.h" /*autoAttackDamageType*/
#include "WeaponDataAsset.h" /*weapon data asset*/
#include "StatusDataAsset.h"
#include "LogUtils.h" /*log*/
#include "Components/SphereComponent.h" /*sphere collision*/
#include "Components/StaticMeshComponent.h" /*static mesh*/
#include "Kismet/GameplayStatics.h" /*GetPlayerCharacter*/
#include "LevelUpManager.h"

// Sets default values
AAutoAttackWeapon::AAutoAttackWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// data asset
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}

	// collision
	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
	RootComponent = BulletCollision;
	BulletCollision->SetSphereRadius(50.f);
	BulletCollision->SetCollisionProfileName(FName("Weapon"));

	// bullet mesh
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// bullet Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/baseWeapon/sm_autoAttack.sm_autoAttack"));
	if (StaticMeshAsset.Succeeded()) {
		BulletMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}
	// bind overlap event
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &AAutoAttackWeapon::OnOverlapBegin);


	//LevelUpManager = CreateDefaultSubobject<ALevelUpManager>(TEXT("LevelUpManager"));
}

// Called when the game starts or when spawned
void AAutoAttackWeapon::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);
	
	Level = LevelUpManager->AutoAttackLevel;
	DamageLevel = LevelUpManager->DamageLevel;
	
	// initialize with data asset
	if (WeaponData) {
		BulletDamage = WeaponData->BaseAttackDamage[Level] * StatusData->Damage[DamageLevel];
		BulletSpeed = WeaponData->BaseAttackSpeed[Level];
		BulletRange = WeaponData->BaseAttackRange[Level];
	}

	BulletLocation = GetActorLocation();
}

// Called every frame
void AAutoAttackWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction{GetActorForwardVector()};
	SetActorLocation(GetActorLocation() + Direction * BulletSpeed * DeltaTime);

	float Distance{static_cast<float>(FVector::Dist(GetActorLocation(), BulletLocation))};

	if (Distance > BulletRange) {
		//LogUtils::Log("Destroy Bullet");
		Destroy();
	}
}

void AAutoAttackWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("AAutoAttackWeapon::OnOverlapBegin", BulletDamage);

		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, this, UAutoAttackDamageType::StaticClass());

		if (5 != LevelUpManager->AutoAttackLevel) {
			Destroy();
		}
	}
}

void AAutoAttackWeapon::DamageLevelUp()

{
	
}

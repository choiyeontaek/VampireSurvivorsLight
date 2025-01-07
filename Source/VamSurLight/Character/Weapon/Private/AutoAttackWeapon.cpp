// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttackWeapon.h"
#include "AutoAttackDamageType.h" /*autoAttackDamageType*/
#include "WeaponDataAsset.h" /*weapon data asset*/
#include "LogUtils.h" /*log*/
#include "Components/SphereComponent.h" /*sphere collision*/
#include "Components/StaticMeshComponent.h" /*static mesh*/
#include "Kismet/GameplayStatics.h" /*GetPlayerCharacter*/

// Sets default values
AAutoAttackWeapon::AAutoAttackWeapon() {
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

	// exp mesh
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// exp Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/baseWeapon/sm_autoAttack.sm_autoAttack"));
	if (StaticMeshAsset.Succeeded()) {
		BulletMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &AAutoAttackWeapon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAutoAttackWeapon::BeginPlay() {
	Super::BeginPlay();

	// initialize with data asset
	if (WeaponData) {
		BulletDamage = WeaponData->BaseAttackDamage;
		BulletSpeed = WeaponData->BaseAttackSpeed;
	}
}

// Called every frame
void AAutoAttackWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	
}

void AAutoAttackWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult) {
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("AAutoAttackWeapon::OnOverlapBegin");

		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());

		Destroy();
	}
}

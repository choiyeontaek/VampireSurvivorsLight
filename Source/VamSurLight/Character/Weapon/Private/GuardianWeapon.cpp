// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianWeapon.h"
#include "LogUtils.h" /*log*/
#include "SkillGuardianDamageType.h"
#include "WeaponDataAsset.h" /*weapon data*/
#include "Components/SphereComponent.h" /*sphere collision*/
#include "Kismet/GameplayStatics.h"	/*apply damage*/


// Sets default values
AGuardianWeapon::AGuardianWeapon()
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
	GuardianCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GuardianCollision"));
	RootComponent = GuardianCollision;
	GuardianCollision->SetSphereRadius(50.f);
	GuardianCollision->SetCollisionProfileName(FName("Weapon"));

	// Guardian mesh
	GuardianMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GuardianMesh"));
	GuardianMesh->SetupAttachment(RootComponent);
	GuardianMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Guardian Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/Boomerang/sm_boomerangAttack.sm_boomerangAttack"));
	if (StaticMeshAsset.Succeeded()) {
		GuardianMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	GuardianCollision->OnComponentBeginOverlap.AddDynamic(this, &AGuardianWeapon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGuardianWeapon::BeginPlay()
{
	Super::BeginPlay();
	// initialize with data asset
	if (WeaponData) {
		GuardianDamage = WeaponData->GuardianDamage[0];
		GuardianSpeed = WeaponData->GuardianSpeed[0];
		GuardianRange = WeaponData->GuardianRange[0];
	}

}

// Called every frame
void AGuardianWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGuardianWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("AGuardianWeapon::OnOverlapBegin");

		UGameplayStatics::ApplyDamage(OtherActor, GuardianDamage, nullptr, nullptr, USkillGuardianDamageType::StaticClass());

		Destroy();
	}
}


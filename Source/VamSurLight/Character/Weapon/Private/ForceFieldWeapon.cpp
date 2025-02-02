// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceFieldWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h"
#include "SkillForceFieldDamageType.h"
#include "WeaponDataAsset.h"
#include "StatusDataAsset.h"
#include "Utils.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"


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
	ForceFieldCollision->SetSphereRadius(250.f);
	ForceFieldCollision->SetCollisionProfileName(FName("Weapon"));

	// Guardian mesh
	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
	ForceFieldMesh->SetupAttachment(RootComponent);
	ForceFieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Guardian Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/forceFieldWeapon/sm_forceField.sm_forceField"));
	if (StaticMeshAsset.Succeeded()) {
		ForceFieldMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	ForceFieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AForceFieldWeapon::OnOverlapBegin);
	ForceFieldCollision->OnComponentEndOverlap.AddDynamic(this, &AForceFieldWeapon::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AForceFieldWeapon::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	Level = LevelUpManager->ForceFieldLevel;
	DamageLevel = LevelUpManager->DamageLevel;

	// initialize with data asset
	if (WeaponData) {
		ForceFieldDamage = WeaponData->ForceFieldDamage[Level] * StatusData->Damage[DamageLevel];
		ForceFieldRange = WeaponData->ForceFieldRange[Level];
	}

	//OverlappedActor = nullptr;

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AForceFieldWeapon::DestroyActor,
	                                       2.f, false);

	GetWorldTimerManager().SetTimer(AttackStartHandle, this, &AForceFieldWeapon::GiveDamage, 0.3f, true);
}

// Called every frame
void AForceFieldWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowPlayer();
}

void AForceFieldWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log("AForceFieldWeapon::OnOverlapBegin", ForceFieldDamage);
		OverlappingActors.Add(OtherActor);
	}
}

void AForceFieldWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		OverlappingActors.Remove(OtherActor);
	}
}

void AForceFieldWeapon::GiveDamage()
{
	for (AActor* Target : OverlappingActors) {
		if (Target && Target->IsValidLowLevel()) {
			AMyCharacter* Character{Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())};
			Character->TotalDamage += ForceFieldDamage;
			UGameplayStatics::ApplyDamage(Target, ForceFieldDamage, GetWorld()->GetFirstPlayerController(), this,
										  USkillForceFieldDamageType::StaticClass());
		}
	}
}

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
		NewLocation.Z -= 80.0f;
		SetActorLocation(NewLocation);
	}
}

void AForceFieldWeapon::DestroyActor()
{
	Destroy();
}

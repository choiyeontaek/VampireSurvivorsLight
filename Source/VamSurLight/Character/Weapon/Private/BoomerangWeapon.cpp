// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerangWeapon.h"
#include "WeaponDataAsset.h"
#include "LevelUpManager.h"
#include "LogUtils.h"
#include "StatusDataAsset.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABoomerangWeapon::ABoomerangWeapon()
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
	BoomerangCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BoomerangCollision"));
	SetRootComponent(BoomerangCollision);
	BoomerangCollision->SetSphereRadius(50.f);
	BoomerangCollision->SetCollisionProfileName(FName("Weapon"));

	// bullet mesh
	BoomerangMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoomerangMesh"));
	BoomerangMesh->SetupAttachment(RootComponent);
	BoomerangMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// bullet Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/Boomerang/batarang.batarang"));
	if (StaticMeshAsset.Succeeded()) {
		BoomerangMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}
	// bind overlap event
	BoomerangCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoomerangWeapon::OnOverlapBegin);

	//LevelUpManager = CreateDefaultSubobject<ALevelUpManager>(TEXT("LevelUpManager"));
}

// Called when the game starts or when spawned
void ABoomerangWeapon::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);

	Level = LevelUpManager->BoomerangLevel;

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	// initialize with data asset
	if (WeaponData) {
		BoomerangDamage = WeaponData->BoomerangDamage[Level] * StatusData->Damage[DamageLevel];
		BoomerangSpeed = WeaponData->BoomerangSpeed[Level];
		BoomerangMovingLength = WeaponData->BoomerangMovingLength[Level];
		BoomerangRange = WeaponData->BoomerangRange[Level];
	}

	InitialLocation = GetActorLocation();
	TargetLocation = FVector::ZeroVector;
	SetTargetLocation();
}

// Called every frame
void ABoomerangWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReachTarget) {
		ReturnWeapon(DeltaTime);
	}
	else {
		MoveToTarget(DeltaTime);
	}
}

void ABoomerangWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log("AAutoAttackWeapon::OnOverlapBegin", BulletDamage);

		//UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());
	}
}

void ABoomerangWeapon::SetTargetLocation()
{
	AActor* NearestMonster{nullptr};
	float NearDistance{FLT_MAX};

	FVector Start{OwningCharacter->GetActorLocation()};
	FVector End{Start + OwningCharacter->GetActorForwardVector() * BoomerangRange};

	FCollisionShape CollisionShape{FCollisionShape::MakeSphere(BoomerangRange)};
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwningCharacter);

	// trace debug
	DrawDebugSphere(GetWorld(), Start, BoomerangRange, 32, FColor::Red, false, 3.0f, 0, 1.0f);
	// sphere trace
	TArray<FHitResult> HitResults;
	if (GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Visibility, CollisionShape,
	                                    QueryParams)) {
		for (const FHitResult& HitResult : HitResults) {
			AActor* HitActor{HitResult.GetActor()};
			// check tag
			if (HitActor && HitActor->ActorHasTag("Monster")) {
				// find monster : green
				DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 3.0f, 0, 3.0f);
				float Distance{static_cast<float>((Start - HitResult.ImpactPoint).Length())};
				if (Distance < NearDistance) {
					NearDistance = Distance;
					NearestMonster = HitActor;
				}
			}
			else {
				// hit but not monster : purple
				DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Purple, false, 3.0f, 0, 3.0f);
			}
		}
	}

	if (NearestMonster) {
		TargetLocation = NearestMonster->GetActorLocation();
		MovingDirection = (TargetLocation - InitialLocation).GetSafeNormal();
	}
	else { // targetLocation == zero vector
		TargetLocation = InitialLocation + OwningCharacter->GetActorForwardVector() * BoomerangRange;
		MovingDirection = OwningCharacter->GetActorForwardVector();
	}
}

void ABoomerangWeapon::MoveToTarget(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + MovingDirection * BoomerangSpeed * DeltaTime);

	float Distance{static_cast<float>(FVector::Dist(GetActorLocation(), InitialLocation))};
	if (Distance > BoomerangRange) {
		TargetLocation = GetActorLocation();
		MovingDirection = (InitialLocation - GetActorLocation()).GetSafeNormal();
		bIsReachTarget = true;
	}
}

void ABoomerangWeapon::ReturnWeapon(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + MovingDirection * BoomerangSpeed * DeltaTime);

	float Distance{static_cast<float>(FVector::Dist(GetActorLocation(), TargetLocation))};
	if (Distance > BoomerangMovingLength) {
		DestroyActor();
	}
}

void ABoomerangWeapon::DestroyActor()
{
	Destroy();
}

void ABoomerangWeapon::LevelUp()
{}

void ABoomerangWeapon::DamageLevelUp()
{}

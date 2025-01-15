// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerangWeapon.h"
#include "WeaponDataAsset.h"
#include "LevelUpManager.h"
#include "StatusDataAsset.h"
#include "LogUtils.h"
#include "Utils.h"
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
	BoomerangMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BoomerangMesh->SetupAttachment(RootComponent);
	BoomerangMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// bullet Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/Boomerang/sm_boomerangAttack.sm_boomerangAttack"));
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
		BoomerangDamage = WeaponData->BoomerangDamage[Level - 1] * StatusData->Damage[DamageLevel];
		BoomerangSpeed = WeaponData->BoomerangSpeed[Level - 1];
		BoomerangMovingLength = WeaponData->BoomerangMovingLength[Level - 1];
		BoomerangRange = WeaponData->BoomerangRange[Level - 1];
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
	int32 NumRays{72};

	AActor* NearestMonster = nullptr;
	float NearDistance = FLT_MAX;

	FVector Start{GetActorLocation()};

	// TArray<FHitResult> Hits;
	// GetWorld()->OverlapMultiByChannel(Hits, GetActorLocation(), FQuat::Identity, ECC_Visibility, );
	// Hits.Sort()
	
	// line trace
	for (int32 i{}; i < NumRays; ++i) {
		float Angle{i * (360.f / NumRays)};
		float RadianAngle{FMath::DegreesToRadians(Angle)};

		FVector End{
			Start + FVector(
				FMath::Cos(RadianAngle) * BoomerangRange,
				FMath::Sin(RadianAngle) * BoomerangRange,
				0.0f
			)
		};

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwningCharacter);
	
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams)) {
			if (HitResult.GetActor()->ActorHasTag(TEXT(""))) {
				
			}
			if (ACharacter* Monster{Cast<ACharacter>(HitResult.GetActor())}) {
				// hit line - green
				DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 3.0f, 0, 1.0f);
				float Distance{HitResult.Distance};
				if (Distance < NearDistance) {
					NearDistance = Distance;
					NearestMonster = Monster;
				}
				TargetLocation = NearestMonster->GetActorLocation();
				MovingDirection = (TargetLocation - InitialLocation).GetSafeNormal();
			}
			else {
				// not monster - purple
				DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 3.0f, 0, 1.0f);
			}
		}
		// nothing hit line - red
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0, 1.0f);
	}
	if (TargetLocation == FVector::ZeroVector) {
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
{
	
}

void ABoomerangWeapon::DamageLevelUp()
{}

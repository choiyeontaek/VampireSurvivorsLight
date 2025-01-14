// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h"
#include "WeaponDataAsset.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
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

	// Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/trainWeapon/sm_trainAttack.sm_trainAttack"));
	if (StaticMeshAsset.Succeeded()) {
		TrainMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// data asset
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}

	LevelUpManager = CreateDefaultSubobject<ALevelUpManager>(TEXT("LevelUpManager"));

	// bind overlap event
	TrainCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrainWeapon::OnOverlapBegin);
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

	StartTrain();
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
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log();

		//UGameplayStatics::ApplyDamage(OtherActor, TrainDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());

		//Destroy();
	}
}

void ATrainWeapon::LevelUp()
{
	
}

void ATrainWeapon::DamageLevelUp()
{
	
}

void ATrainWeapon::StartTrain()
{
	//AActor* OwningActor = GetOwner();
	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (OwningCharacter) {
		//TrainStartDistance = 800.f;
		// make random angle 0 ~ 360
		float RandomAngle{2 * PI * FMath::RandRange(0.f, 1.f)};
		// Polar to Cartesian
		RandomPosition = {
			TrainStartDistance * FMath::Cos(RandomAngle), TrainStartDistance * (FMath::Sin(RandomAngle)),
			OwningCharacter->GetActorLocation().Z
		};
		// position by player
		RandomPosition.X += OwningCharacter->GetActorLocation().X;
		RandomPosition.Y += OwningCharacter->GetActorLocation().Y;

		MovingDirection = OwningCharacter->GetActorLocation() - RandomPosition;
		MovingDirection.Normalize();

		SetActorLocation(RandomPosition);
	}
}

void ATrainWeapon::DestroyActor()
{
	Destroy();
}

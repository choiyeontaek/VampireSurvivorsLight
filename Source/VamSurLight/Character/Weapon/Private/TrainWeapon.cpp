// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h"
#include "SkillTrainDamageType.h"
#include "StatusDataAsset.h"
#include "WeaponDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATrainWeapon::ATrainWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// collision
	TrainCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrainCollision"));
	SetRootComponent(TrainCollision);
	TrainCollision->SetBoxExtent(FVector(425.f, 140.f, 400.f));
	TrainCollision->SetCollisionProfileName(FName("Weapon"));

	// TrainMesh
	TrainMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TrainSkeletalMesh"));
	TrainMesh->SetupAttachment(RootComponent);
	TrainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TrainMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	// Mesh load
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset
	(TEXT(
		"/Game/player/weapon/trainWeapon/train_-_british_rail_class_08_rail_blue_livery.train_-_british_rail_class_08_rail_blue_livery"));
	if (skeletalMeshAsset.Succeeded()) {
		TrainMesh->SetSkeletalMesh(skeletalMeshAsset.Object);
	}

	TrainMesh->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimAsset
	(TEXT(
		"/Game/player/weapon/trainWeapon/train_-_british_rail_class_08_rail_blue_livery_Anim.train_-_british_rail_class_08_rail_blue_livery_Anim"));
	if (AnimAsset.Succeeded()) {
		TrainAnimation = AnimAsset.Object;
	}

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
	//LevelUpManager = CreateDefaultSubobject<ALevelUpManager>(TEXT("LevelUpManager"));

	// bind overlap event
	TrainCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrainWeapon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATrainWeapon::BeginPlay()
{
	Super::BeginPlay();
	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);

	Level = LevelUpManager->TrainLevel;
	DamageLevel = LevelUpManager->DamageLevel;

	// initialize with data asset
	if (WeaponData) {
		TrainDamage = WeaponData->TrainDamage[Level] * StatusData->Damage[DamageLevel];
		TrainSpeed = WeaponData->TrainSpeed[Level];
		TrainMovingLength = WeaponData->TrainMovingLength[Level];
		TrainStartDistance = WeaponData->TrainStartDistance[Level];
	}

	if (TrainAnimation) {
		TrainMesh->PlayAnimation(TrainAnimation, true);
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
		LogUtils::Log("ATrainWeapon::OnOverlapBegin", TrainDamage);

		UGameplayStatics::ApplyDamage(OtherActor, TrainDamage, GetWorld()->GetFirstPlayerController(), this, USkillTrainDamageType::StaticClass());
	}
}

void ATrainWeapon::LevelUp()
{}

void ATrainWeapon::DamageLevelUp()
{}

void ATrainWeapon::StartTrain()
{
	//AActor* OwningActor = GetOwner();
	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (OwningCharacter) {
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

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(RandomPosition, OwningCharacter->GetActorLocation());
		SetActorRotation(SpawnRotation);
	}
}

void ATrainWeapon::DestroyActor()
{
	Destroy();
}

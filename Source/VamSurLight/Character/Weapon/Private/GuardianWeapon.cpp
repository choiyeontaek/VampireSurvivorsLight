// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianWeapon.h"

#include "LevelUpManager.h"
#include "LogUtils.h" /*log*/
#include "SkillGuardianDamageType.h"
#include "WeaponDataAsset.h" /*weapon data*/
#include "StatusDataAsset.h"
#include "Components/SphereComponent.h" /*sphere collision*/
#include "GameFramework/Character.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"	/*apply damage*/
#include "Kismet/KismetMathLibrary.h" /*FindLookAtRotation*/


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
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}
	// collision
	GuardianCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GuardianCollision"));
	RootComponent = GuardianCollision;
	GuardianCollision->SetSphereRadius(70.f);
	GuardianCollision->SetCollisionProfileName(FName("Weapon"));

	// Guardian mesh
	GuardianMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GuardianMesh"));
	GuardianMesh->SetupAttachment(RootComponent);
	GuardianMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Guardian Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
		(TEXT("/Game/player/weapon/guadianWeapon/boomerang_freefire.boomerang_freefire"));
	if (StaticMeshAsset.Succeeded()) {
		GuardianMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	GuardianCollision->OnComponentBeginOverlap.AddDynamic(this, &AGuardianWeapon::OnOverlapBegin);
	GuardianCollision->OnComponentEndOverlap.AddDynamic(this, &AGuardianWeapon::OnOverlapEnd);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

// Called when the game starts or when spawned
void AGuardianWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	AActor* FoundActorLevelUpManager{UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass())};
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);

	Level = LevelUpManager->GuardianLevel;
	DamageLevel = LevelUpManager->DamageLevel;

	// initialize with data asset
	if (WeaponData) {
		GuardianDamage = WeaponData->GuardianDamage[Level] * StatusData->Damage[DamageLevel];
		GuardianSpeed = WeaponData->GuardianSpeed[Level];
		GuardianRange = WeaponData->GuardianRange[Level];
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AGuardianWeapon::DestroyActor, 3.0f, false);

	// calculate weapon location based on character location
	InitialOffset = (GetActorLocation() - OwningCharacter->GetActorLocation()).GetSafeNormal();
	// calculate absolute angle
	InitialAngle = FMath::Atan2(InitialOffset.Y, InitialOffset.X);

	if (RotatingMovement) {
		RotatingMovement->RotationRate = FRotator(0.f, 360.f, 0.f);
	}
}

// Called every frame
void AGuardianWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwningCharacter) {
		CurrentAngle += GuardianSpeed * DeltaTime;

		float TotalAngle{InitialAngle + CurrentAngle};
		FVector NewLocation{
			OwningCharacter->GetActorLocation() + FVector(FMath::Cos(TotalAngle), FMath::Sin(TotalAngle),
			                                              InitialOffset.Z) * GuardianRange
		};
		SetActorLocation(NewLocation);

		// rotate to player
		//FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),OwningCharacter->GetActorLocation());
		//SetActorRotation(NewRotation);
	}
}

void AGuardianWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log("AGuardianWeapon::OnOverlapBegin", GuardianDamage);
		OverlappedActor = OtherActor;

		GetWorld()->GetTimerManager().SetTimer(AttackStartHandle, this, &AGuardianWeapon::GiveDamage, 0.5f, true);
	}
}

void AGuardianWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		OverlappedActor = nullptr;

		GetWorld()->GetTimerManager().ClearTimer(AttackStartHandle);
	}
}

void AGuardianWeapon::GiveDamage()
{
	if (OverlappedActor) {
		UGameplayStatics::ApplyDamage(OverlappedActor, GuardianDamage, GetWorld()->GetFirstPlayerController(), this,
		                              USkillGuardianDamageType::StaticClass());
	}
}

void AGuardianWeapon::DestroyActor()
{
	Destroy();
}

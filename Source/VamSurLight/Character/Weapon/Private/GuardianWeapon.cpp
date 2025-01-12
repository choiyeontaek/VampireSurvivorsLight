// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianWeapon.h"
#include "LogUtils.h" /*log*/
#include "SkillGuardianDamageType.h"
#include "WeaponDataAsset.h" /*weapon data*/
#include "Components/SphereComponent.h" /*sphere collision*/
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
		(TEXT("/Game/player/weapon/guadianWeapon/sm_guadianAttack.sm_guadianAttack"));
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

	InitialLocation = GetActorLocation();
	CurrentAngle = 0.f;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AGuardianWeapon::DestroyActor, 3.0f, false);
}

// Called every frame
void AGuardianWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	AActor* OwningActor = GetOwner();
	if (OwningActor) {
		FVector InitialOffset = InitialLocation - OwningActor->GetActorLocation();
		float InitialAngle = FMath::Atan2(InitialOffset.Y, InitialOffset.X);
		
		CurrentAngle += GuardianSpeed * DeltaTime;
		
		float TotalAngle = InitialAngle + CurrentAngle;
		FVector NewLocation = OwningActor->GetActorLocation() + FVector(FMath::Cos(TotalAngle), FMath::Sin(TotalAngle), InitialOffset.Z) * GuardianRange;
		SetActorLocation(NewLocation);

		// rotate to player
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),OwningActor->GetActorLocation());
		SetActorRotation(NewRotation);
	}
}

void AGuardianWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("AGuardianWeapon::OnOverlapBegin");

		UGameplayStatics::ApplyDamage(OtherActor, GuardianDamage, nullptr, nullptr,
		                              USkillGuardianDamageType::StaticClass());
	}
}

void AGuardianWeapon::DestroyActor()
{
	Destroy();
}

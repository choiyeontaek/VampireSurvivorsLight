// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGuardian.h"
#include "GuardianWeapon.h"
#include "LogUtils.h"	/*log*/
#include "StatusDataAsset.h" /*status data*/
#include "SynergyManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" /*get actor*/
#include "Kismet/KismetMathLibrary.h"	/*findLookAtLocation*/


// Sets default values
ASkillGuardian::ASkillGuardian()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// status data asset
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}

	// set weapon class
	GuardianWeapon = AGuardianWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASkillGuardian::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);

	// initialize data
	if (StatusData) {
		Projectile = StatusData->Projectile[0];
	}

	Attack(Projectile + 3);
}

// Called every frame
void ASkillGuardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkillGuardian::Attack(int32 Count)
{
	if (SynergyManager) {
		if (SynergyManager->CheckSynergy(EWeaponType::AutoAttack, EStatusType::CoolTimeUpdate)) {
			LogUtils::Log("Yes Synergy");
		}
		else {
			LogUtils::Log("No Synergy");
		}
	}

	if (GuardianWeapon && GetWorld()) {
		for (int32 i{}; i < Count; i++) {
			float Angle = (360.0f / Count) * i;
			FVector SpawnLocation = OwningCharacter->GetActorLocation() + 
				FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f) * 200.f;
			FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, OwningCharacter->GetActorLocation());

			//LogUtils::Log("location", SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AGuardianWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AGuardianWeapon>(GuardianWeapon, SpawnLocation, SpawnRotation, SpawnParams);
			if (SpawnedWeapon) {
				SpawnedWeapon->SetOwner(OwningCharacter);
			}
		}
	}
}

void ASkillGuardian::DestroyActor()
{
	Destroy();
}

void ASkillGuardian::LevelUp()
{}


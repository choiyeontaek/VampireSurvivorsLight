// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGuardian.h"
#include "GuardianWeapon.h"
#include "LogUtils.h"	/*log*/
#include "StatusDataAsset.h" /*status data*/
#include "SynergyManager.h"
#include "Kismet/GameplayStatics.h" /*get actor*/


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

	// set auto attack weapon class
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
		Projectile = StatusData->Projectile;
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
}

void ASkillGuardian::DestroyActor()
{
	Destroy();
}


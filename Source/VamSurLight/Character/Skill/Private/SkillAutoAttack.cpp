// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAutoAttack.h"
#include "AutoAttackWeapon.h"
#include "LevelUpManager.h"
#include "LogUtils.h"	/*log*/
#include "StatusDataAsset.h"
#include "WeaponDataAsset.h"
#include "SynergyManager.h"	/*synergyCheck*/
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"	/*find look at rotation*/

// Sets default values
ASkillAutoAttack::ASkillAutoAttack()
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
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}

	// set auto attack weapon class
	AutoAttackWeapon = AAutoAttackWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASkillAutoAttack::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);

	AActor* FoundActorLevelUpManager = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelUpManager::StaticClass());
	LevelUpManager = Cast<ALevelUpManager>(FoundActorLevelUpManager);

	WeaponLevel = LevelUpManager->AutoAttackLevel;
	ProjectileLevel = 0;
	// initialize data
	if (StatusData) {
		StatusProjectile = StatusData->Projectile[ProjectileLevel];
		WeaponProjectile = WeaponData->BaseAttackProjectile[WeaponLevel];
	}

	Attack(StatusProjectile + WeaponProjectile);
}

// Called every frame
void ASkillAutoAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotateToMouse) {
		RotateToMouse();
	}
}

void ASkillAutoAttack::Attack(int32 Count)
{
	LogUtils::Log("ASkillAutoAttack::AutoAttack");

	if (SynergyManager) {
		if (SynergyManager->CheckSynergy(EWeaponType::AutoAttack)) {
			LogUtils::Log("Yes Synergy");
		}
		else {
			LogUtils::Log("No Synergy");
		}
	}

	if (OwningCharacter) {
		if (AutoAttackWeapon && GetWorld()) {
			// rotate character
			bRotateToMouse = true;
			// spawn attack 
			for (int32 i{}; i < Count; i++) {
				FTimerHandle WaitHandle;
				GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]() {
					FVector SpawnLocation = OwningCharacter->GetActorLocation() + OwningCharacter->
						GetActorForwardVector() * 100.f;
					FRotator SpawnRotation = OwningCharacter->GetActorRotation();

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					AAutoAttackWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AAutoAttackWeapon>(
						AutoAttackWeapon, SpawnLocation, SpawnRotation, SpawnParams);
				}, 0.1f * (i + 1), false);
			}
			// if end attack, stop rotate
			float TotalAttackTime{0.1f * Count};
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ASkillAutoAttack::StopRotateToMouse,
			                                       TotalAttackTime, false);
		}
	}
}

void ASkillAutoAttack::DestroyActor()
{
	Destroy();
}

void ASkillAutoAttack::LevelUp()
{}

void ASkillAutoAttack::RotateToMouse()
{
	if (!OwningCharacter) {
		return;
	}

	// rotate character to mouseDirection
	if (APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController())) {
		FHitResult HitResult;
		if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult)) {
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
				OwningCharacter->GetActorLocation(), HitResult.Location);
			NewRotation.Pitch = 0.0f;
			NewRotation.Roll = 0.0f;
			OwningCharacter->SetActorRotation(NewRotation);
		}
	}
}

void ASkillAutoAttack::StopRotateToMouse()
{
	// stop rotate player
	bRotateToMouse = false;

	// destroy actor
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ASkillAutoAttack::DestroyActor, 1.f, false);
}

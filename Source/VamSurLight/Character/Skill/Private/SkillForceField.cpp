// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillForceField.h"
#include "StatusDataAsset.h"
#include "ForceFieldWeapon.h"
#include "GameFramework/Character.h"


// Sets default values
ASkillForceField::ASkillForceField()
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
	
	ForceFieldWeapon = AForceFieldWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASkillForceField::BeginPlay()
{
	Super::BeginPlay();
	
	OwningCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();

	FVector SpawnLocation = OwningCharacter->GetActorLocation();
	FRotator SpawnRotation = OwningCharacter->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AForceFieldWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AForceFieldWeapon>(ForceFieldWeapon, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedWeapon) {
		SpawnedWeapon->SetOwner(OwningCharacter);
	}
}

// Called every frame
void ASkillForceField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkillForceField::LevelUp()
{
	
}

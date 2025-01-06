// Fill out your copyright notice in the Description page of Project Settings.


#include "Experience.h"
#include "DropItemDataAsset.h" /*data asset*/
#include "LogUtils.h" /*log*/
#include "Components/SphereComponent.h" /*sphere collision*/
#include "Components/StaticMeshComponent.h" /*static mesh*/
#include "MyCharacter.h" /*character*/
#include "Kismet/GameplayStatics.h" /*GetPlayerCharacter*/


// Sets default values
AExperience::AExperience() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// data asset
	static ConstructorHelpers::FObjectFinder<UDropItemDataAsset> DropItemDataAsset
	(TEXT("/Game/Data/dataAsset_dropItem.dataAsset_dropItem"));
	if (DropItemDataAsset.Succeeded()) {
		DropItemData = DropItemDataAsset.Object;
	}

	// collision
	ExpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExpCollision"));
	RootComponent = ExpCollision;
	ExpCollision->SetSphereRadius(50.f);

	// exp mesh
	ExpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExpMesh"));
	ExpMesh->SetupAttachment(RootComponent);
	ExpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// exp Mesh load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset
	(TEXT("/Game/DropItem/experience/sm_exp.sm_exp"));
	if (StaticMeshAsset.Succeeded()) {
		ExpMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	// bind overlap event
	ExpCollision->OnComponentBeginOverlap.AddDynamic(this, &AExperience::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AExperience::BeginPlay() {
	Super::BeginPlay();
	
	Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// initialize with data asset
	if (DropItemData) {
		ExpAmount = DropItemData->ExpAmount;
		ExpCollision->SetSphereRadius(DropItemData->ExpCollisionRange);
	}
}

// Called every frame
void AExperience::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AExperience::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("AExperience::OnOverlapBegin");

		if (Character) {
			Character->AddExperience(ExpAmount);
		}
		
		// move mesh to player and destroy
		Destroy();
	}
}
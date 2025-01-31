// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainWeapon.generated.h"

UCLASS()
class VAMSURLIGHT_API ATrainWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrainWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void StartTrain();
	
	// weapon data asset
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UWeaponDataAsset* WeaponData;
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	
	UPROPERTY(EditDefaultsOnly)
	class ALevelUpManager* LevelUpManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class UBoxComponent* TrainCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* TrainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	class UAnimationAsset* TrainAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainStartDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainMovingLength;
	
	UPROPERTY()
	class ACharacter* OwningCharacter;

	FVector InitialLocation;
	FVector MovingDirection;
	FVector RandomPosition;
	
	int32 Level;
	int32 DamageLevel;

	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	void DestroyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Height;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector StartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector EndLocation;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoomerangWeapon.generated.h"

UCLASS()
class VAMSURLIGHT_API ABoomerangWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoomerangWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetTargetLocation();
	void MoveToTarget(float DeltaTime);
	void ReturnWeapon(float DeltaTime);
	void LevelUp();
	void DamageLevelUp();

	
	// weapon data asset
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UWeaponDataAsset* WeaponData;
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	
	UPROPERTY(EditDefaultsOnly)
	class ALevelUpManager* LevelUpManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* BoomerangCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* BoomerangMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BoomerangDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BoomerangSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BoomerangMovingLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BoomerangRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector InitialLocation;
	UPROPERTY()
	class ACharacter* OwningCharacter;

	FVector TargetLocation;
	FVector MovingDirection;
	
	int32 Level;
	int32 DamageLevel;

	bool bIsReachTarget{false};
	
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	void DestroyActor();
};

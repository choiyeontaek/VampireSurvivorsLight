﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	
	// weapon data asset
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UWeaponDataAsset* WeaponData;
	
	UPROPERTY(EditDefaultsOnly)
	class ULevelUpManager* LevelUpManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* TrainCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* TrainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainStartDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainMovingLength;
	

	FVector InitialLocation;
	FVector MovingDirection;
	FVector RandomPosition;
	
	int32 Level;

	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	void DestroyActor();
};
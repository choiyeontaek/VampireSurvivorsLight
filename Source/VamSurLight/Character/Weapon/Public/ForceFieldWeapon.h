﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceFieldWeapon.generated.h"

UCLASS()
class VAMSURLIGHT_API AForceFieldWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AForceFieldWeapon();

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* ForceFieldCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* ForceFieldMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ForceFieldDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ForceFieldRange;

	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	void FollowPlayer();
	void DestroyActor();
};
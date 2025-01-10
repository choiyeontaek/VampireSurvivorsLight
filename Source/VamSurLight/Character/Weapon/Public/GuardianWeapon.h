// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuardianWeapon.generated.h"

UCLASS()
class VAMSURLIGHT_API AGuardianWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGuardianWeapon();

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
	class USphereComponent* GuardianCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* GuardianMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float GuardianDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float GuardianSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float GuardianRange;
};

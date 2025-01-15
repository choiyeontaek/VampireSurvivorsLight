// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBoomerang.generated.h"

UCLASS()
class VAMSURLIGHT_API ASkillBoomerang : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkillBoomerang();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Attack(int32 Count);
	
	void DestroyActor();
	
	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data")
	class UWeaponDataAsset* WeaponData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BoomerangMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY()
	class ACharacter* OwningCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ABoomerangWeapon> BoomerangWeapon;

	// synergy check
	UPROPERTY()
	class ASynergyManager* SynergyManager;
	
	// timer handle
	FTimerHandle DestroyTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float StatusProjectile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float WeaponProjectile;
	
	void LevelUp();
	int32 ProjectileLevel;
	int32 WeaponLevel;

	UPROPERTY(EditDefaultsOnly)
	class ALevelUpManager* LevelUpManager;
};

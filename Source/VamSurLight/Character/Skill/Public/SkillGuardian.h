// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillGuardian.generated.h"

UCLASS()
class VAMSURLIGHT_API ASkillGuardian : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkillGuardian();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Attack(int32 Count);
	
	void DestroyActor();

	UPROPERTY()
	ACharacter* OwningCharacter;
	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AGuardianWeapon> GuardianWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Projectile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	// synergy check
	UPROPERTY()
	class ASynergyManager* SynergyManager;

	// timer handle
	FTimerHandle DestroyTimerHandle;

	void LevelUp();
	int32 Level;
};

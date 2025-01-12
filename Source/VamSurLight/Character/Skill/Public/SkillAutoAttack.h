// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillAutoAttack.generated.h"

UCLASS()
class VAMSURLIGHT_API ASkillAutoAttack : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkillAutoAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Attack(int32 Count);

	// rotating character
	void RotateToMouse();
	void StopRotateToMouse();
	void DestroyActor();

	UPROPERTY()
	ACharacter* OwningCharacter;
	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AAutoAttackWeapon> AutoAttackWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Projectile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	// synergy check
	UPROPERTY()
	class ASynergyManager* SynergyManager;

	// timer handle
	FTimerHandle DestroyTimerHandle;
	
	bool bRotateToMouse;
};

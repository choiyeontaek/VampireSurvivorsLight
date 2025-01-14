// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoAttackWeapon.generated.h"

UCLASS()
class VAMSURLIGHT_API AAutoAttackWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoAttackWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void LevelUp();
	void DamageLevelUp();
	
	// weapon data asset
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UWeaponDataAsset* WeaponData;

	UPROPERTY(EditDefaultsOnly)
	class ALevelUpManager* LevelUpManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* BulletCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* BulletMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector BulletLocation;

	int32 Level;
};

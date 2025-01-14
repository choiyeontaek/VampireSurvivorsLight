// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillForceField.generated.h"

UCLASS()
class VAMSURLIGHT_API ASkillForceField : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkillForceField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* FieldMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	
	UPROPERTY()
	class ACharacter* OwningCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AForceFieldWeapon> ForceFieldWeapon;

	// timer handle
	FTimerHandle DestroyTimerHandle;

	void LevelUp();
	int32 Level;
};

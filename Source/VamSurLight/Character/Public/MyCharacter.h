// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

enum class EWeaponType : uint8;

UCLASS()
class VAMSURLIGHT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void UpdateHealthUI();
	void UpdateExpUI();
	void RegenerateHealth();
	void AddHealth(float AddHP);
	void AddExperience(float ExpAmount);
	void LevelUp();
	void AutoAttack();
	void StartAttack(EWeaponType WeaponType);
	
	// widget
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> CharacterMainUIClass;
	UPROPERTY()
	UUserWidget* CharacterWidget;
	UPROPERTY()
	class UCharacterHealthUI* HealthUI;
	UPROPERTY()
	class UCharacterExpUI* ExpUI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent *MainSpringArm;	// spring arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FollowCamera;	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent *PlayerSkeletalMesh;	// skeletal mesh

	// character data asset
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UCharacterDataAsset* CharacterData;

	// weapon data asset
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data")
	class UWeaponDataAsset* WeaponData;

	// status data asset
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;

	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillAutoAttack> SkillAutoAttack;
	
	// timer	
	UPROPERTY()
	FTimerHandle ActionTimerHandle;
	UPROPERTY()
	FTimerHandle HealthRegenerateHandle;
	
	// synergy check
	UPROPERTY()
	class ASynergyManager* SynergyManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "movement")
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float BaseAttackDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float MaxExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float Exp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float HealthRegeneration;
};
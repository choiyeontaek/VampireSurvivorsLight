// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillChooseUI.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

enum class EStatusType : uint8;
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
	
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void UpdateHealthUI();
	void UpdateExpUI();
	void RegenerateHealth();
	void AddHealth(float AddHP);
	void AddExperience(float ExpAmount);
	UFUNCTION(BlueprintCallable)
	void LevelUp();
	void StartAttack(EWeaponType WeaponType);
	void AutoAttack();
	void GuardianAttack();
	void TrainAttack();
	void BoomerangAttack();
	void ForceFieldAttack();
	void StatusLevelUp(EStatusType Status);
	void CharacterDie();
	UFUNCTION(BlueprintCallable)
	void CharacterRevive();
	void UpdateSkillUI(FCardOption CardOption, int32 level);

public:
	/// widget
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> CharacterMainUIClass;
	UPROPERTY()
	class UUserWidget* CharacterWidget;
	UPROPERTY()
	class UCharacterHealthUI* HealthUI;
	UPROPERTY()
	class UCharacterExpUI* ExpUI;
	UPROPERTY()
	class UCharacterSkillListUI* CharacterSkillUI;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> GameUIClass;
	UPROPERTY()
	class UDieUI* DieUI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent *MainSpringArm;	// spring arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FollowCamera;	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent *PlayerSkeletalMesh;	// skeletal mesh
	
	/// data assets
	UPROPERTY(EditDefaultsOnly, Category = "Character Data")
	class UCharacterDataAsset* CharacterData;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data")
	class UWeaponDataAsset* WeaponData;
	UPROPERTY(EditDefaultsOnly, Category = "Status Data")
	class UStatusDataAsset* StatusData;
	UPROPERTY(EditDefaultsOnly, Category = "Card Data")
	class UCardDataAsset* CardData;

	UPROPERTY(BlueprintReadOnly)
	class ALevelUpManager* LevelUpManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillAutoAttack> SkillAutoAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillGuardian> SkillGuardianAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillForceField> SkillForceFieldAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillTrain> SkillTrainAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ASkillBoomerang> SkillBoomerangAttack;
	
	/// timer	
	UPROPERTY()
	FTimerHandle AutoAttackTimerHandle;
	UPROPERTY()
	FTimerHandle GuardianAttackTimerHandle;
	UPROPERTY()
	FTimerHandle TrainAttackTimerHandle;
	UPROPERTY()
	FTimerHandle BoomerangAttackTimerHandle;
	UPROPERTY()
	FTimerHandle ForceFieldAttackTimerHandle;
	UPROPERTY()
	FTimerHandle HealthRegenerateHandle;
	
	/// synergy check
	UPROPERTY()
	class ASynergyManager* SynergyManager;

	float DefaultSpeed{600.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "movement")
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float MaxExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float Exp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float CoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float HealthRegeneration{0.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "state")
	float AddSpeed;
	
	/// status level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MyLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HealthRegenerationLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CoolTimeLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MovementSpeedLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MaxHealthLevel{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DamageLevel{0};

	int32 AutoAttackUIIndex;
	int32 BoomerangUIIndex;
	int32 TrainUIIndex;
	int32 ForceFieldUIIndex;
	int32 GuardianUIIndex;
	
	int32 CoolTimeUIIndex;
	int32 MovementSpeedUIIndex;
	int32 DamageUpdateUIIndex;
	int32 MaxHealthUIIndex;
	int32 HealthRegenerationUIIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AutoAttackCoolTime{1};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BoomerangCoolTime{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrainCoolTime{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ForceFieldCoolTime{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float GuardianCoolTime{0};
	
};
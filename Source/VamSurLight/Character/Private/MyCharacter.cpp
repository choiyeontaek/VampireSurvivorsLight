// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"   /*spring arm*/
#include "Camera/CameraComponent.h" /*camera*/
#include "Engine/DamageEvents.h" /*damage event*/
#include "GameFramework/CharacterMovementComponent.h" /*movement component*/
#include "Components/CapsuleComponent.h" /*overlap*/
#include "LogUtils.h" /*log*/
#include "CharacterDataAsset.h" /*character data asset*/
#include "WeaponDataAsset.h" /*weapon data asset*/
#include "BaseDamageType.h"	/*baseDamage*/
#include "AutoAttackDamageType.h"	/*autoAttack*/
#include "SkillGuardianDamageType.h"	/*skillGuardian*/
#include "Blueprint/UserWidget.h"	/*widget*/
#include "CharacterHealthUI.h" /*healthUI*/
#include "CharacterExpUI.h" /*expUI*/
#include "SkillAutoAttack.h"	/**/
#include "SynergyManager.h"	/*check synergy*/
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h" /*FOutputDeviceNull */
#include "StatusDataAsset.h" /*status data*/

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// collision settings
	GetCapsuleComponent()->SetCollisionProfileName(FName("Player"));

	// springArm
	MainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	MainSpringArm->SetupAttachment(RootComponent);
	MainSpringArm->TargetArmLength = 2000.0f;
	MainSpringArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	MainSpringArm->bDoCollisionTest = false;

	// camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(MainSpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// skeletalMesh
	PlayerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	PlayerSkeletalMesh->SetupAttachment(RootComponent);
	PlayerSkeletalMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -85.f), FRotator(0.f, -90.f, 0.f));
	PlayerSkeletalMesh->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	PlayerSkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// skeletalMesh load
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset
		(TEXT("/Game/player/model/sm_character_graves.sm_character_graves"));
	if (skeletalMeshAsset.Succeeded()) {
		PlayerSkeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object);
	}

	// set anim class
	static ConstructorHelpers::FClassFinder<UAnimInstance> animBP
		(TEXT("/Game/player/animation/bp_characterAnimation"));
	if (animBP.Succeeded()) {
		PlayerSkeletalMesh->SetAnimInstanceClass(animBP.Class);
	}

	// character data asset
	static ConstructorHelpers::FObjectFinder<UCharacterDataAsset> CharacterDataAsset
		(TEXT("/Game/Data/dataAsset_character.dataAsset_character"));
	if (CharacterDataAsset.Succeeded()) {
		CharacterData = CharacterDataAsset.Object;
	}

	// weapon data asset
	static ConstructorHelpers::FObjectFinder<UWeaponDataAsset> WeaponDataAsset
		(TEXT("/Game/Data/dataAsset_weapon.dataAsset_weapon"));
	if (WeaponDataAsset.Succeeded()) {
		WeaponData = WeaponDataAsset.Object;
	}

	// status data asset
	static ConstructorHelpers::FObjectFinder<UStatusDataAsset> StatusDataAsset
		(TEXT("/Game/Data/dataAsset_status.dataAsset_status"));
	if (StatusDataAsset.Succeeded()) {
		StatusData = StatusDataAsset.Object;
	}

	// camera, character movement setting
	MainSpringArm->bInheritYaw = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// bind overlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);

	// get widget
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget
		(TEXT("/Game/player/widget/widget_characterMainUI.widget_characterMainUI_C"));
	if (Widget.Succeeded()) {
		CharacterMainUIClass = Widget.Class;
	}

	// set class
	SkillAutoAttack = ASkillAutoAttack::StaticClass();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsDead = false;

	// initialize with data asset
	if (CharacterData) {
		MaxHealth = CharacterData->CharacterMaxHealth;
		Health = CharacterData->CharacterHealth;
		MaxExp = CharacterData->CharacterMaxExp;
		Exp = CharacterData->CharacterExp;
		HealthRegeneration = CharacterData->CharacterHealthRegeneration;
	}

	// add viewport widget
	CharacterWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), CharacterMainUIClass);
	if (CharacterWidget) {
		CharacterWidget->AddToViewport();

		HealthUI = Cast<UCharacterHealthUI>(CharacterWidget->GetWidgetFromName(TEXT("widget_healthUI")));
		if (HealthUI) {
			UpdateHealthUI();
		}

		ExpUI = Cast<UCharacterExpUI>(CharacterWidget->GetWidgetFromName(TEXT("widget_expUI")));
		if (ExpUI) {
			UpdateExpUI();
		}
	}

	LogUtils::Log("need remove"); // when player dead

	// start auto attack
	StartAttack(EWeaponType::AutoAttack);

	// health generation
	GetWorldTimerManager().SetTimer(HealthRegenerateHandle, this, &AMyCharacter::RegenerateHealth, 3.0f, true);

	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);
	// acquire autoAttackWeapon
	SynergyManager->AcquireWeapon(EWeaponType::AutoAttack);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity{GetVelocity()};
	Speed = Velocity.Size();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//LogUtils::Log("AMyCharacter::OnOverlapBegin ");
		//UGameplayStatics::ApplyDamage(OtherActor, BaseAttackDamage, nullptr, nullptr, UBaseDamageType::StaticClass());
		//UGameplayStatics::ApplyDamage(OtherActor, BaseAttackDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage{Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser)};
	//LogUtils::Log("Character::TakeDamage");

	// damageType -> my custom damageType
	const UDamageType* DamageTypeRaw{
		DamageEvent.DamageTypeClass
			? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>()
			: GetDefault<UDamageType>()
	};
	const UBaseDamageType* DamageType{Cast<UBaseDamageType>(DamageTypeRaw)};

	if (!bIsDead && DamageType) {
		if (DamageType->IsA(UAutoAttackDamageType::StaticClass())) {
			LogUtils::Log("Character::TakeDamage - AutoAttackDamageType");
			DamageType->ApplyDamageEffect(this, DamageAmount, EventInstigator, DamageCauser);
			Health -= Damage;
		}
		else if (DamageType->IsA(USkillGuardianDamageType::StaticClass())) {
			LogUtils::Log("Character::TakeDamage - SkillGuardianDamageType");
			DamageType->ApplyDamageEffect(this, DamageAmount, EventInstigator, DamageCauser);
			Health -= Damage;
		}
		else if (DamageType->GetClass()->GetFName() == FName("BP_DamageType_RangeAttack_C")) {
			LogUtils::Log("Character::TakeDamage - RangeAttack");

			FOutputDeviceNull ar;
			const FString command = TEXT("RangeAttack");

			const_cast<UBaseDamageType*>(DamageType)->CallFunctionByNameWithArguments(*command, ar, nullptr, true);

			Health -= Damage;
		}

		UpdateHealthUI();

		if (Health <= 0) {
			//LogUtils::Log("PlayerDead");
			bIsDead = true;
		}
	}

	return Damage;
}

void AMyCharacter::UpdateHealthUI()
{
	if (HealthUI) {
		HealthUI->UpdateHealthBar();
	}
}

void AMyCharacter::UpdateExpUI()
{
	if (ExpUI) {
		ExpUI->UpdateExpBar();
	}
}

void AMyCharacter::RegenerateHealth()
{
	LogUtils::Log("Character::RegenerateHealth");
	
	if (Health < MaxHealth) {
		Health += HealthRegeneration;
		Health = FMath::Min(Health, MaxHealth);

		UpdateHealthUI();
	}
}

void AMyCharacter::AddHealth(float AddHp)
{
	return;
}

void AMyCharacter::AddExperience(float ExpAmount)
{
	Exp += ExpAmount;
	//LogUtils::Log("AMyCharacter::AddExperience", Exp);
	// level up
	if (Exp > MaxExp) {
		Exp = Exp - MaxExp;
		LevelUp();
	}
	if (ExpUI) {
		ExpUI->UpdateExpBar();
	}
}

void AMyCharacter::LevelUp()
{
	MaxExp *= CharacterData->CharacterExpMul;
	ExpUI->UpdateLevelText();

	//LogUtils::Log("AMyCharacter::LevelUp", MaxExp);
}

void AMyCharacter::AutoAttack() // character auto attack
{
	//LogUtils::Log("AMyCharacter::AutoAttack");

	if (SkillAutoAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 100.f)};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillAutoAttack* SkillActor = GetWorld()->SpawnActor<ASkillAutoAttack>(
			SkillAutoAttack, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyCharacter::StartAttack(EWeaponType WeaponType)
{
	// start auto attack
	switch (WeaponType) {
	case EWeaponType::AutoAttack:
		GetWorldTimerManager().SetTimer(ActionTimerHandle, this, &AMyCharacter::AutoAttack, 2.5f, true);
		break;

	default:
		break;
	}
}

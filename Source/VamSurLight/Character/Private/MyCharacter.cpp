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
#include "CardDataAsset.h"
#include "SkillGuardianDamageType.h"	/*skillGuardian*/
#include "Blueprint/UserWidget.h"	/*widget*/
#include "CharacterHealthUI.h" /*healthUI*/
#include "CharacterExpUI.h" /*expUI*/
#include "CharacterSkillListUI.h"
#include "DieUI.h"
#include "LevelUpManager.h"	/*handleLevelUp*/
#include "SkillAutoAttack.h"
#include "SkillBoomerang.h"
#include "SkillForceField.h"
#include "SkillGuardian.h"
#include "SkillTrain.h"
#include "SynergyManager.h"	/*check synergy*/
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h" /*FOutputDeviceNull */
#include "StatusDataAsset.h" /*status data*/
#include "Chaos/ChaosPerfTest.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

class UCardDataAsset;
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

	static ConstructorHelpers::FObjectFinder<UCardDataAsset> CardDataAsset
		(TEXT("/Game/Data/dataAsset_card.dataAsset_card"));
	if (CardDataAsset.Succeeded()) {
		CardData = CardDataAsset.Object;
	}

	// camera, character movement setting
	MainSpringArm->bInheritYaw = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// bind overlap event
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);

	// get widget
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget
		(TEXT("/Game/player/widget/widget_characterMainUI.widget_characterMainUI_C"));
	if (Widget.Succeeded()) {
		CharacterMainUIClass = Widget.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> DieWidgetClass
		(TEXT("/Game/GameWidget/widget_Die.widget_Die_C"));
	if (DieWidgetClass.Succeeded()) {
		GameUIClass = DieWidgetClass.Class;
	}


	// set class
	SkillAutoAttack = ASkillAutoAttack::StaticClass();
	SkillGuardianAttack = ASkillGuardian::StaticClass();
	SkillForceFieldAttack = ASkillForceField::StaticClass();
	SkillTrainAttack = ASkillTrain::StaticClass();
	SkillBoomerangAttack = ASkillBoomerang::StaticClass();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize synergyManager
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASynergyManager::StaticClass());
	SynergyManager = Cast<ASynergyManager>(FoundActor);

	LevelUpManager = GetWorld()->SpawnActor<ALevelUpManager>(ALevelUpManager::StaticClass());

	bIsDead = false;

	// initialize with data asset
	if (CharacterData) {
		MaxHealth = CharacterData->CharacterMaxHealth;
		Health = CharacterData->CharacterHealth;
		MaxExp = CharacterData->CharacterMaxExp;
		Exp = CharacterData->CharacterExp;
		CoolTime = StatusData->CoolTime[CoolTimeLevel];
		AddSpeed = StatusData->MovementSpeed[MovementSpeedLevel];
	}

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * ((100 + AddSpeed) / 100);

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

		CharacterSkillUI = Cast<UCharacterSkillListUI>(CharacterWidget->GetWidgetFromName(TEXT("widget_skillList")));
	}

	LogUtils::Log("need remove"); // when player dead

	// start auto attack
	StartAttack(EWeaponType::AutoAttack);

	// health generation
	GetWorldTimerManager().SetTimer(HealthRegenerateHandle, this, &AMyCharacter::RegenerateHealth, 3.0f, true);
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
{}

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
		if (DamageType->GetClass()->GetFName() == FName("BP_DamageType_RangeAttack_C")) {
			LogUtils::Log("Character::TakeDamage - BP_DamageType_RangeAttack_C");
			FOutputDeviceNull Ar;
			// function name in blueprint
			const FString Command{TEXT("RangeAttack")};
			const_cast<UBaseDamageType*>(DamageType)->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
			Health -= Damage;
		}
		else if (DamageType->GetClass()->GetFName() == FName("BP_DamageType_LongRangeAttackangeAttack_C")) {
			LogUtils::Log("Character::TakeDamage - BP_DamageType_LongRangeAttackangeAttack");
			FOutputDeviceNull Ar;
			const FString Command{TEXT("LongRangeAttack")};
			const_cast<UBaseDamageType*>(DamageType)->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
			Health -= Damage;
		}
		else if (DamageType->GetClass()->GetFName() == FName("BP_DamageType_LongSkill_C")) {
			LogUtils::Log("Character::TakeDamage - BP_DamageType_LongSkill");
			FOutputDeviceNull Ar;
			const FString Command{TEXT("LongRangeSkillAttack")};
			const_cast<UBaseDamageType*>(DamageType)->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
			Health -= Damage;
		}
		else if (DamageType->GetClass()->GetFName() == FName("BP_DamageType_BossAttack_C")) {
			LogUtils::Log("Character::TakeDamage - BP_DamageType_BossAttack");
			FOutputDeviceNull Ar;
			const FString Command{TEXT("BossAttack")};
			const_cast<UBaseDamageType*>(DamageType)->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
			Health -= Damage;
		}

		UpdateHealthUI();

		if (Health <= 0) {
			//LogUtils::Log("PlayerDead");
			CharacterDie();
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
	//LogUtils::Log("Character::RegenerateHealth", HealthRegeneration[Level - 1]);

	if (Health < MaxHealth) {
		Health += HealthRegeneration;
		Health = FMath::Min(Health, MaxHealth);

		UpdateHealthUI();
	}
}

void AMyCharacter::AddHealth(float AddHp)
{
	Health += AddHp;
	Health = FMath::Min(Health, MaxHealth);
	UpdateHealthUI();
}

void AMyCharacter::AddExperience(float ExpAmount)
{
	Exp += ExpAmount;
	//LogUtils::Log("AMyCharacter::AddExperience", Exp);

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
	++MyLevel;

	LevelUpManager->HandleLevelUp(this);

	LogUtils::Log("AMyCharacter::LevelUp", MyLevel);
}

void AMyCharacter::StartAttack(EWeaponType WeaponType)
{
	// start auto attack
	switch (WeaponType) {
	case EWeaponType::AutoAttack:
		SynergyManager->AcquireWeapon(EWeaponType::AutoAttack);
		GetWorldTimerManager().SetTimer(AutoAttackTimerHandle, this, &AMyCharacter::AutoAttack,
		                                2.5f * ((100 - CoolTime) / 100), true);
		break;
	case EWeaponType::Guardian:
		SynergyManager->AcquireWeapon(EWeaponType::Guardian);
		GetWorldTimerManager().SetTimer(GuardianAttackTimerHandle, this, &AMyCharacter::GuardianAttack,
		                                6.5f * ((100 - CoolTime) / 100), true);
		break;
	case EWeaponType::ForceField:
		SynergyManager->AcquireWeapon(EWeaponType::ForceField);
		GetWorldTimerManager().SetTimer(ForceFieldAttackTimerHandle, this, &AMyCharacter::ForceFieldAttack,
		                                3.f * ((100 - CoolTime) / 100), true);
		break;
	case EWeaponType::Train:
		SynergyManager->AcquireWeapon(EWeaponType::Train);
		GetWorldTimerManager().SetTimer(TrainAttackTimerHandle, this, &AMyCharacter::TrainAttack,
		                                2.8f * ((100 - CoolTime) / 100), true);
		break;
	case EWeaponType::Boomerang:
		SynergyManager->AcquireWeapon(EWeaponType::Boomerang);
		GetWorldTimerManager().SetTimer(BoomerangAttackTimerHandle, this, &AMyCharacter::BoomerangAttack,
		                                3.f * ((100 - CoolTime) / 100), true);
		break;
	default:
		break;
	}
}

void AMyCharacter::AutoAttack() // character auto attack
{
	if (SkillAutoAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 100.f)};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillAutoAttack* SkillActor = GetWorld()->SpawnActor<ASkillAutoAttack>(
			SkillAutoAttack, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyCharacter::GuardianAttack()
{
	if (SkillGuardianAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation()};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillGuardian* SkillActor = GetWorld()->SpawnActor<ASkillGuardian>(
			SkillGuardianAttack, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyCharacter::TrainAttack()
{
	if (SkillTrainAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation()};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillTrain* SkillActor = GetWorld()->SpawnActor<ASkillTrain>(
			SkillTrainAttack, SpawnLocation, SpawnRotation, SpawnParams);
		if (SkillActor) {
			SkillActor->SetOwner(this);
		}
	}
}

void AMyCharacter::BoomerangAttack()
{
	if (SkillBoomerangAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation()};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillBoomerang* SkillActor = GetWorld()->SpawnActor<ASkillBoomerang>(
			SkillBoomerangAttack, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyCharacter::ForceFieldAttack()
{
	if (SkillForceFieldAttack && GetWorld()) {
		FVector SpawnLocation{GetActorLocation()};
		FRotator SpawnRotation{GetActorRotation()};

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASkillForceField* SkillActor = GetWorld()->SpawnActor<ASkillForceField>(
			SkillForceFieldAttack, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyCharacter::StatusLevelUp(EStatusType Status)
{
	switch (Status) {
	case EStatusType::CoolTimeUpdate:
		++CoolTimeLevel;
		if (1 == CoolTimeLevel) {
			SynergyManager->AcquireStatus(EStatusType::CoolTimeUpdate);
		}
		CoolTime = StatusData->CoolTime[CoolTimeLevel];

		if (SynergyManager->CheckWeapon(EWeaponType::AutoAttack)) {
			GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
			GetWorldTimerManager().SetTimer(AutoAttackTimerHandle, this, &AMyCharacter::AutoAttack,
			                                2.5f * ((100 - CoolTime) / 100), true);
		}
		if (SynergyManager->CheckWeapon(EWeaponType::Boomerang)) {
			GetWorld()->GetTimerManager().ClearTimer(BoomerangAttackTimerHandle);
			GetWorldTimerManager().SetTimer(BoomerangAttackTimerHandle, this, &AMyCharacter::BoomerangAttack,
			                                3.f * ((100 - CoolTime) / 100), true);
		}
		if (SynergyManager->CheckWeapon(EWeaponType::Guardian)) {
			GetWorld()->GetTimerManager().ClearTimer(GuardianAttackTimerHandle);
			GetWorldTimerManager().SetTimer(GuardianAttackTimerHandle, this, &AMyCharacter::GuardianAttack,
			                                6.5f * ((100 - CoolTime) / 100), true);
		}
		if (SynergyManager->CheckWeapon(EWeaponType::Train)) {
			GetWorld()->GetTimerManager().ClearTimer(TrainAttackTimerHandle);
			GetWorldTimerManager().SetTimer(TrainAttackTimerHandle, this, &AMyCharacter::TrainAttack,
			                                2.8f * ((100 - CoolTime) / 100), true);
		}
		if (SynergyManager->CheckWeapon(EWeaponType::ForceField)) {
			GetWorld()->GetTimerManager().ClearTimer(ForceFieldAttackTimerHandle);
			GetWorldTimerManager().SetTimer(ForceFieldAttackTimerHandle, this, &AMyCharacter::ForceFieldAttack,
			                                3.f * ((100 - CoolTime) / 100), true);
		}
		break;
	case EStatusType::HealthRegenerationUpdate:
		++HealthRegenerationLevel;
		if (1 == HealthRegenerationLevel) {
			SynergyManager->AcquireStatus(EStatusType::HealthRegenerationUpdate);
		}
		HealthRegeneration = StatusData->HealthRegeneration[HealthRegenerationLevel];
		break;
	case EStatusType::MaxHealthUpdate:
		++MaxHealthLevel;
		if (1 == MaxHealthLevel) {
			SynergyManager->AcquireStatus(EStatusType::MaxHealthUpdate);
		}
		MaxHealth += StatusData->MaxHealth[MaxHealthLevel];
		Health += StatusData->MaxHealth[MaxHealthLevel];
		UpdateHealthUI();
		break;
	case EStatusType::MovementSpeedUpdate:
		++MovementSpeedLevel;
		if (1 == MovementSpeedLevel) {
			SynergyManager->AcquireStatus(EStatusType::MovementSpeedUpdate);
		}
		AddSpeed = StatusData->CoolTime[MovementSpeedLevel];
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * ((100 + AddSpeed) / 100);
		break;
	case EStatusType::DamageUpdate:
		++DamageLevel;
		if (1 == DamageLevel) {
			SynergyManager->AcquireStatus(EStatusType::DamageUpdate);
		}
		break;
	default:
		break;
	}
}

void AMyCharacter::CharacterDie()
{
	bIsDead = true;

	// delay

	DieUI = Cast<UDieUI>(CreateWidget(GetWorld()->GetFirstPlayerController(), GameUIClass));

	if (DieUI) {
		DieUI->AddToViewport();
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) {
		PlayerController->SetPause(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AMyCharacter::CharacterRevive()
{
	// delay

	bIsDead = false;

	Health = MaxHealth;
	UpdateHealthUI();
}

void AMyCharacter::UpdateSkillUI(FCardOption CardOption, int32 level)
{
	if (CardOption.bIsWeapon) {
		switch (CardOption.WeaponType) {
		case EWeaponType::None:
			break;
		case EWeaponType::AutoAttack:
			CharacterSkillUI->WeaponOneText->SetText(
				UKismetTextLibrary::Conv_IntToText(level));
			if (5 == level) {
				CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->AutoAttackImageFinal);
			}

			break;
		case EWeaponType::ForceField:
			if (false == SynergyManager->CheckWeapon(EWeaponType::ForceField)) {
				if (0 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->ForceFieldImage);
					ForceFieldUIIndex = 1;
				}
				else if (1 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->ForceFieldImage);
					ForceFieldUIIndex = 2;
				}
				else if (2 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->ForceFieldImage);
					ForceFieldUIIndex = 3;
				}
			}

			if (1 == ForceFieldUIIndex) {
				CharacterSkillUI->WeaponOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->ForceFieldImageFinal);
				}
			}
			else if (2 == ForceFieldUIIndex) {
				CharacterSkillUI->WeaponTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->ForceFieldImageFinal);
				}
			}
			else if (3 == ForceFieldUIIndex) {
				CharacterSkillUI->WeaponThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->ForceFieldImageFinal);
				}
			}
			break;
		case EWeaponType::Boomerang:
			if (false == SynergyManager->CheckWeapon(EWeaponType::Boomerang)) {
				if (0 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->BoomerangImage);
					BoomerangUIIndex = 1;
				}
				else if (1 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->BoomerangImage);
					BoomerangUIIndex = 2;
				}
				else if (2 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->BoomerangImage);
					BoomerangUIIndex = 3;
				}
			}

			if (1 == BoomerangUIIndex) {
				CharacterSkillUI->WeaponOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->BoomerangImageFinal);
				}
			}
			else if (2 == BoomerangUIIndex) {
				CharacterSkillUI->WeaponTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->BoomerangImageFinal);
				}
			}
			else if (3 == BoomerangUIIndex) {
				CharacterSkillUI->WeaponThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->BoomerangImageFinal);
				}
			}
			break;
		case EWeaponType::Guardian:
			if (false == SynergyManager->CheckWeapon(EWeaponType::Guardian)) {
				if (0 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->GuardianImage);
					GuardianUIIndex = 1;
				}
				else if (1 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->GuardianImage);
					GuardianUIIndex = 2;
				}
				else if (2 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->GuardianImage);
					GuardianUIIndex = 3;
				}
			}

			if (1 == GuardianUIIndex) {
				CharacterSkillUI->WeaponOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->GuardianImageFinal);
				}
			}
			else if (2 == GuardianUIIndex) {
				CharacterSkillUI->WeaponTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->GuardianImageFinal);
				}
			}
			else if (3 == GuardianUIIndex) {
				CharacterSkillUI->WeaponThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->GuardianImageFinal);
				}
			}
			break;
		case EWeaponType::Train:
			if (false == SynergyManager->CheckWeapon(EWeaponType::Train)) {
				if (0 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->TrainImage);
					TrainUIIndex = 1;
				}
				else if (1 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->TrainImage);
					TrainUIIndex = 2;
				}
				else if (2 == SynergyManager->GetWeaponCount()) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->TrainImage);
					TrainUIIndex = 3;
				}
			}

			if (1 == TrainUIIndex) {
				CharacterSkillUI->WeaponOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponOneImage->SetBrushFromTexture(CardData->TrainImageFinal);
				}
			}
			else if (2 == TrainUIIndex) {
				CharacterSkillUI->WeaponTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponTwoImage->SetBrushFromTexture(CardData->TrainImageFinal);
				}
			}
			else if (3 == TrainUIIndex) {
				CharacterSkillUI->WeaponThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
				if (5 == level) {
					CharacterSkillUI->WeaponThreeImage->SetBrushFromTexture(CardData->TrainImageFinal);
				}
			}
			break;
		case EWeaponType::MAX:
			break;
		}
	}
	else {
		switch (CardOption.StatusType) {
		case EStatusType::None:
			break;
		case EStatusType::DamageUpdate:
			if (false == SynergyManager->CheckStatus(EStatusType::DamageUpdate)) {
				if (0 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusOneImage->SetBrushFromTexture(CardData->DamageUpdateImage);
					DamageUpdateUIIndex = 1;
				}
				else if (1 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusTwoImage->SetBrushFromTexture(CardData->DamageUpdateImage);
					DamageUpdateUIIndex = 2;
				}
				else if (2 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusThreeImage->SetBrushFromTexture(CardData->DamageUpdateImage);
					DamageUpdateUIIndex = 3;
				}
			}

			if (1 == DamageUpdateUIIndex) {
				CharacterSkillUI->StatusOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (2 == DamageUpdateUIIndex) {
				CharacterSkillUI->StatusTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (3 == DamageUpdateUIIndex) {
				CharacterSkillUI->StatusThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			break;
		case EStatusType::CoolTimeUpdate:
			if (false == SynergyManager->CheckStatus(EStatusType::CoolTimeUpdate)) {
				if (0 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusOneImage->SetBrushFromTexture(CardData->CoolTimeUpdateImage);
					CoolTimeUIIndex = 1;
				}
				else if (1 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusTwoImage->SetBrushFromTexture(CardData->CoolTimeUpdateImage);
					CoolTimeUIIndex = 2;
				}
				else if (2 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusThreeImage->SetBrushFromTexture(CardData->CoolTimeUpdateImage);
					CoolTimeUIIndex = 3;
				}
			}

			if (1 == CoolTimeUIIndex) {
				CharacterSkillUI->StatusOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (2 == CoolTimeUIIndex) {
				CharacterSkillUI->StatusTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (3 == CoolTimeUIIndex) {
				CharacterSkillUI->StatusThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			break;
		case EStatusType::HealthRegenerationUpdate:
			if (false == SynergyManager->CheckStatus(EStatusType::HealthRegenerationUpdate)) {
				if (0 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusOneImage->SetBrushFromTexture(CardData->HealthRegenerationUpdateImage);
					HealthRegenerationUIIndex = 1;
				}
				else if (1 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusTwoImage->SetBrushFromTexture(CardData->HealthRegenerationUpdateImage);
					HealthRegenerationUIIndex = 2;
				}
				else if (2 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusThreeImage->SetBrushFromTexture(CardData->HealthRegenerationUpdateImage);
					HealthRegenerationUIIndex = 3;
				}
			}

			if (1 == HealthRegenerationUIIndex) {
				CharacterSkillUI->StatusOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (2 == HealthRegenerationUIIndex) {
				CharacterSkillUI->StatusTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (3 == HealthRegenerationUIIndex) {
				CharacterSkillUI->StatusThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			break;
		case EStatusType::MovementSpeedUpdate:
			if (false == SynergyManager->CheckStatus(EStatusType::MovementSpeedUpdate)) {
				if (0 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusOneImage->SetBrushFromTexture(CardData->MovementSpeedUpdateImage);
					MovementSpeedUIIndex = 1;
				}
				else if (1 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusTwoImage->SetBrushFromTexture(CardData->MovementSpeedUpdateImage);
					MovementSpeedUIIndex = 2;
				}
				else if (2 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusThreeImage->SetBrushFromTexture(CardData->MovementSpeedUpdateImage);
					MovementSpeedUIIndex = 3;
				}
			}

			if (1 == MovementSpeedUIIndex) {
				CharacterSkillUI->StatusOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (2 == MovementSpeedUIIndex) {
				CharacterSkillUI->StatusTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (3 == MovementSpeedUIIndex) {
				CharacterSkillUI->StatusThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			break;
		case EStatusType::MaxHealthUpdate:
			if (false == SynergyManager->CheckStatus(EStatusType::MaxHealthUpdate)) {
				if (0 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusOneImage->SetBrushFromTexture(CardData->MaxHealthUpdateImage);
					MaxHealthUIIndex = 1;
				}
				else if (1 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusTwoImage->SetBrushFromTexture(CardData->MaxHealthUpdateImage);
					MaxHealthUIIndex = 2;
				}
				else if (2 == SynergyManager->GetStatusCount()) {
					CharacterSkillUI->StatusThreeImage->SetBrushFromTexture(CardData->MaxHealthUpdateImage);
					MaxHealthUIIndex = 3;
				}
			}

			if (1 == MaxHealthUIIndex) {
				CharacterSkillUI->StatusOneText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (2 == MaxHealthUIIndex) {
				CharacterSkillUI->StatusTwoText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			else if (3 == MaxHealthUIIndex) {
				CharacterSkillUI->StatusThreeText->SetText(
					UKismetTextLibrary::Conv_IntToText(level));
			}
			break;
		case EStatusType::MAX:
			break;
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"   /*spring arm*/
#include "Camera/CameraComponent.h" /*camera*/
#include "Engine/DamageEvents.h" /*damage event*/
#include "GameFramework/CharacterMovementComponent.h" /*movement component*/
#include "Kismet/GameplayStatics.h" /*apply damage*/
#include "Components/CapsuleComponent.h" /*overlap*/
#include "LogUtils.h" /*log*/
#include "CharacterDataAsset.h" /*data asset*/
#include "BaseDamageType.h"	/*baseDamage*/
#include "AutoAttackDamageType.h"	/*autoAttack*/
#include "SkillGuardianDamageType.h"	/*skillGuardian*/

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// collision settings
	
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

	// data asset
	static ConstructorHelpers::FObjectFinder<UCharacterDataAsset> CharacterDataAsset
	(TEXT("/Game/player/dataAsset_character.dataAsset_character"));
	if (CharacterDataAsset.Succeeded()) {
		CharacterData = CharacterDataAsset.Object;
	}
	
    // camera, character movement setting
    MainSpringArm->bInheritYaw = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// bind overlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsDead = false;

	// initialize with data asset
	if (CharacterData) {
		Health = CharacterData->CharacterHealth;
		BaseAttackDamage = CharacterData->BaseAttackDamage;
	}
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
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		LogUtils::Log("Overlap");
		//UGameplayStatics::ApplyDamage(OtherActor, BaseAttackDamage, nullptr, nullptr, UBaseDamageType::StaticClass());
		UGameplayStatics::ApplyDamage(OtherActor, BaseAttackDamage, nullptr, nullptr, UAutoAttackDamageType::StaticClass());
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage{Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser)};
	LogUtils::Log("character::takeDamage - TakeDamage");

	const UDamageType* DamageTypeRaw{DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>()};
	const UBaseDamageType* DamageType = Cast<UBaseDamageType>(DamageTypeRaw);
	
	if (!bIsDead && DamageType) {
		if (DamageType->IsA(UAutoAttackDamageType::StaticClass())) {
			LogUtils::Log("character::takeDamage - AutoAttackDamageType");
			DamageType->ApplyDamageEffect(this, DamageAmount, EventInstigator, DamageCauser);
			Health -= Damage;
		}
		else if (DamageType->IsA(USkillGuardianDamageType::StaticClass())) {
			LogUtils::Log("character::takeDamage - SkillGuardianDamageType");
			DamageType->ApplyDamageEffect(this, DamageAmount, EventInstigator, DamageCauser);
		}
		
		if (Health <= 0) {
			LogUtils::Log("playerDead");
			bIsDead = true;
		}
	}
	
	return Damage;
}

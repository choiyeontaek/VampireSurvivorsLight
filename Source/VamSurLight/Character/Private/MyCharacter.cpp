// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"   /*spring arm*/
#include "Camera/CameraComponent.h" /*camera*/
#include "GameFramework/CharacterMovementComponent.h" /*movement component*/

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // springArm
    mainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    mainSpringArm->SetupAttachment(RootComponent);
    mainSpringArm->TargetArmLength = 2000.0f;
    mainSpringArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
    mainSpringArm->bDoCollisionTest = false;
    
    // camera
    followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    followCamera->SetupAttachment(mainSpringArm, USpringArmComponent::SocketName);
    followCamera->bUsePawnControlRotation = false;

    // skeletalMesh
    playerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
    playerSkeletalMesh->SetupAttachment(RootComponent);
    playerSkeletalMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -85.f), FRotator(0.f, -90.f, 0.f));
    playerSkeletalMesh->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	playerSkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

    // skeletalMesh load
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset
    (TEXT("/Game/player/model/sm_character_graves.sm_character_graves"));
    if (skeletalMeshAsset.Succeeded()) {
        playerSkeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object);
    }

	// set anim class
	static ConstructorHelpers::FClassFinder<UAnimInstance> animBP
	(TEXT("/Game/player/animation/bp_characterAnimation"));
	if (animBP.Succeeded()) {
		playerSkeletalMesh->SetAnimInstanceClass(animBP.Class);
	}
	
    // camera, character movement setting
    mainSpringArm->bInheritYaw = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

	// default player states
	isDead = false;
	health = 100.f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector velocity{GetVelocity()};
	speed = velocity.Size();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMyCharacter::TakeDamage(float damageAmount, struct FDamageEvent const& damageEvent,class AController* eventInstigator, AActor* damageCauser)
{
	float damage{Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser)};
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Damage"));
	
	if (!isDead) {
		health -= damage;
		if (health <= 0) {
			isDead = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("DEAD"));
		}
	}
	
	return damage;
}
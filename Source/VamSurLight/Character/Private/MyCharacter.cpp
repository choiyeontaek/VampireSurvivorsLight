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
    playerSkeletalMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.9f));

    // skeletalMesh load
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshAsset
    (TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
    if (skeletalMeshAsset.Succeeded()) {
        playerSkeletalMesh->SetSkeletalMesh(skeletalMeshAsset.Object);
    }

    // camera, character movement setting
    mainSpringArm->bInheritYaw = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
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

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


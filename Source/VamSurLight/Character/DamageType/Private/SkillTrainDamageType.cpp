// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTrainDamageType.h"
#include "LevelUpManager.h"
#include "LogUtils.h"
#include "MyCharacter.h"
#include "WeaponDataAsset.h"

void USkillTrainDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                             AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);

	LogUtils::Log("USkillTrainDamageType::ApplyDamageEffect");

	AMyCharacter* Character{Cast<AMyCharacter>(InstigatedBy->GetCharacter())};
	// if (Character->LevelUpManager->TrainLevel < 5) {
	// 	//FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
	// 	//DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	// }
	// else if (Character->LevelUpManager->TrainLevel == 5) {
	// explosion
	FVector DamagedLocation{DamagedActor->GetActorLocation()};
	FVector CauserLocation{DamageCauser->GetActorLocation()};
	DamagedLocation.Z = 0.f;
	CauserLocation.Z = 0.f;
	float Distance{static_cast<float>((DamagedLocation - CauserLocation).Size())};
	//LogUtils::Log("Distance", Distance);
	// F = explosionEnergy / r^2
	float Force{Character->WeaponData->TrainForce / FMath::Pow(Distance, 2)};
	//LogUtils::Log("Force", Force);
	// a = f / m
	float Acceleration{Force / 10.f};
	//LogUtils::Log("Acceleration", Acceleration);
	// v0 = a * t
	float Speed{Acceleration * Character->WeaponData->TrainTime};
	//LogUtils::Log("Speed", Speed);
	// h = v0^2 / ( 2 * g )
	// multiply 100 for 'M' to 'CM'
	Height = FMath::Pow(Speed, 2) / (2.f * 9.8f) * 100.f;
	//LogUtils::Log("Height", Height);

	// d = v0^2 / ( 2 * friction * gravity )
	// multiply 100 for 'M' to 'CM'
	float MoveDistance{FMath::Pow(Speed, 2) / (2.f * 0.5f * 9.8f) * 100.f};
	//LogUtils::Log("MovingDistance", MoveDistance);

	StartLocation = DamagedActor->GetActorLocation();
	FVector Direction{(DamagedActor->GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()};
   
	EndLocation = StartLocation + Direction * MoveDistance;
	//DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * MoveDistance);
	//}
	
}



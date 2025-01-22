// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGuardianDamageType.h"
#include "LogUtils.h" /*log*/
#include "MyCharacter.h" /*character*/
#include "LevelUpManager.h"

void USkillGuardianDamageType::ApplyDamageEffect_Implementation(AActor* DamagedActor, float Damage,
                                                                AController* InstigatedBy, AActor* DamageCauser) const
{
	Super::ApplyDamageEffect_Implementation(DamagedActor, Damage, InstigatedBy, DamageCauser);
	
	LogUtils::Log("SkillGuardianDamageType::ApplyDamageEffect");
	

	//if (LevelUpManager->AutoAttackLevel < 5) {
		AMyCharacter* Character = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
		FVector Direction{(DamagedActor->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal()};
		DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);

	// }
	// else if (LevelUpManager->AutoAttackLevel == 5) {
	// 	AMyCharacter* Character = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//
	// 	FVector Direction{(DamagedActor->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal()};
	// 	DamagedActor->SetActorLocation(DamagedActor->GetActorLocation() + Direction * 100.f);
	//
	// }
}

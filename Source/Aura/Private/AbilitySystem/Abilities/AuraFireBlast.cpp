// Copyrights Skorokhod.


#include "AbilitySystem/Abilities/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		"<Title>FIRE Blast</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"

		"<Default>Launches %d</>"
		"<Default>fire balls in all directions, each coming back and</>"
		"<Default>exploding upon return, causing </>"
		"<Damage>%d</>"

		"<Default> radial fire damage with a chance to burn</>\n\n"
	), Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);

}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return GetDescription(Level);
}


TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> Rotators  = UAuraAbilitySystemBPLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.0f, NumFireBalls);

	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFormClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		
		FireBall->ExplosionDamageEffectParams = MakeDamageEffectParamsFormClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}

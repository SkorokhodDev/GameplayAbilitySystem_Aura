// Copyrights Skorokhod.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"

#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches a bolt of fire, exploding on impact and dealing </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>\n\n"
			), Level, ManaCost,Cooldown, ScaledDamage);
	}
	else // Launches multiple fire bolts if level > 1
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d bolts of fire, exploding on impact and dealing </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>\n\n"), FMath::Min(Level, NumProjectiles),
			ManaCost, Cooldown, ScaledDamage, Level);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
			"<Title>Next Level:</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d bolts of fire, exploding on impact and dealing </>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
		), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation,
	const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	// Spawn projectile on a server
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
		return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		SocketTag);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	//Rotation.Pitch = 0.f; // Flying straight

	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	
	int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UAuraAbilitySystemBPLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		check(ProjectileClass);
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFormClassDefaults(nullptr);

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			// for garbage collection of this object
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			// this is a weak pointer, so our USceneComponent won`t be cleaned by garbage collector.
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->FinishSpawning(SpawnTransform);
	}

}

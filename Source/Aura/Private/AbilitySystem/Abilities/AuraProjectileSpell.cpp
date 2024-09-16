// Copyrights Skorokhod.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Actor/AuraProjectile.h"
#include <Interaction/CombatInterface.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"



void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 3.0f);

}


void UAuraProjectileSpell::SpawnProjectile(
	const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// Spawn projectile on a server
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
		return;

	if (!DamageEffectClass)
	{
		checkf(0, L"Invalid DamageEffectClass");
		return;
	}


	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), 
		SocketTag);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	//Rotation.Pitch = 0.f; // Flying straight

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	check(ProjectileClass);
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFormClassDefaults(nullptr);


	Projectile->FinishSpawning(SpawnTransform);
}

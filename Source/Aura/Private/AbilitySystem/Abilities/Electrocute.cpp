// Copyrights Skorokhod.


#include "AbilitySystem/Abilities/Electrocute.h"

#include "Kismet/KismetSystemLibrary.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"

#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(
			TEXT
			(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning,</>"
			"connecting with a target, repeatedly causing"
			"<Damage>%d</>"
			"<Default> lightning damage with a chance to stun</>\n\n"
		), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else // Launches multiple fire bolts if level > 1
	{
		return FString::Printf(
			TEXT
			(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning, </>"
			"propagating to %d additional targets nearby, causing "
			"<Damage>%d</>"
			"<Default> lightning damage with a chance to stun</>\n\n"
		), 
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(
		TEXT
		(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning, </>"
			"propagating to %d additional targets nearby, causing "
			"<Damage>%d</>"
			"<Default> lightning damage with a chance to stun</>\n\n"
		),
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
}

// Copyrights Skorokhod.


#include "AbilitySystem/Abilities/ArcaneShards.h"

#include "Kismet/KismetSystemLibrary.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"

#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(
			TEXT
			(
				"<Title>ARCANE SHARDS</>\n\n"
				"<Small>Level: </><Level>%d</>\n"
				"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
				"<Small>Cooldown: </><Cooldown>%.1f</>\n"
				"<Default>Summon shards of Arcane energy,</>"
				"causing radial damage of "
				"<Damage>%d</>"
				"<Default> at the shard origins.</>\n\n"
			), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else // Launches multiple fire bolts if level > 1
	{
		return FString::Printf(
			TEXT
			(
				"<Title>ARCANE SHARDS</>\n\n"
				"<Small>Level: </><Level>%d</>\n"
				"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
				"<Small>Cooldown: </><Cooldown>%.1f</>\n"
				"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
				"<Damage>%d</>"
				"<Default>.</>\n\n"
			),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards - 1), ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(
		TEXT
		(
			"<Title>ARCANE SHARDS</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
			"<Damage>%d</>"
			"<Default>.</>\n\n"
		),
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards - 1), ScaledDamage);
}

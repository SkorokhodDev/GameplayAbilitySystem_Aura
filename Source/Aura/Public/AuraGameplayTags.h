// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
* AuraGameplayTags
* 
* Singleton containing native Gameplay Tags
*/

struct FAuraGameplayTags
{
public:
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/* Resistances */
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lighting;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/* Damage */
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	/* Abilities */
	FGameplayTag Abilities_Attack;


	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	//TArray<FGameplayTag> DamageTypes;

	FGameplayTag Effects_HitReact;
private:
	static FAuraGameplayTags GameplayTags;

public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();


};
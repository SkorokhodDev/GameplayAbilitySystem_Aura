// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf <UGameplayEffect> DamageEffectClass;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	//FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage> TaggedMontages);

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};

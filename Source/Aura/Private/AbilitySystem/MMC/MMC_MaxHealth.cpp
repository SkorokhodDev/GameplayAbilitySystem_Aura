// Copyrights Skorokhod.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

//Aura
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
		VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

		VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
		VigorDef.bSnapshot = false; // bSnapshot = true -> capture attribute when it is created

		RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tabs from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.0f; // local Vigor will have the same value as our Vigor in attribute set
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor); 
	Vigor = FMath::Max<float>(Vigor, 0.0f); // If Vigor < 0, we set it to 0

	int32 PlayerLevel = 1;
	if(Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());		
	}
	
	//	Base value
	return 80.0f + 2.5f * Vigor + 10.0f * PlayerLevel;
}

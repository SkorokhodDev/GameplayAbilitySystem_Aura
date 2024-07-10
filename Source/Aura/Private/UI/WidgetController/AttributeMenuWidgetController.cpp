// Copyrights Skorokhod.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
// Engine

//Aura
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
#include <Player/AuraPlayerState.h>
#include <AbilitySystem/AuraAbilitySystemComponent.h>

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// this function updates our values
	UAuraAttributeSet* AuraAS = GetAuraAS();
	for (auto& Pair : AuraAS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AuraAS](const FOnAttributeChangeData& Data) 
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	AAuraPlayerState* const AuraPS = GetAuraPS();

	AuraPS->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points)
	{
			AttributePointsChangedDelegate.Broadcast(Points);
	});
		
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = GetAuraAS();

	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	AAuraPlayerState* const AuraPS = GetAuraPS();
	// Get attribute points that we have on start
	AttributePointsChangedDelegate.Broadcast(AuraPS->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

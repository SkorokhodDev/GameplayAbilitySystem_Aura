// Copyrights Skorokhod.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include <Player/AuraPlayerState.h>

#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Create our Widget
	const UAuraAttributeSet* const AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAttributeSet)
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/* Player State */
	AAuraPlayerState* const AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);	
		}
	);

	/* Attribute Set */
	const UAuraAttributeSet* const AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	if (AuraAttributeSet)
	{
		// Bind delegate, when our value is changed, and broadcast to our own delegate
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); }
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue); }
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetManaAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); }
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data) {OnMaxManaChanged.Broadcast(Data.NewValue); }
		);

		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			if(AuraASC->bStartupAbilitiesGiven)
			{
				OnInitializeStartupAbilities(AuraASC);
			}
			else
			{
				AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
			}
				// add & or this to call a member function GetDataTableRowByTag
			AuraASC->EffectAssetTagsDelegate.AddLambda(
				[this](const FGameplayTagContainer& AssetTags)
				{
					for (const FGameplayTag& Tag : AssetTags)
					{
						FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
						if (Tag.MatchesTag(MessageTag))
						{
							//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
							//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

							// Finding our Row from DataTable by Tag.Name
							const FUIWidgetRow* const Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
							MessageWidterRowDelegate.Broadcast(*Row);
						}
					}
				}
			);
		}
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbilitySignature BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfoData->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	} );

	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const AAuraPlayerState* const AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* const LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out AuraPlayerState Blueprint"));

	int32 level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 maxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(level <= maxLevel && level > 0)
	{
		const int32 currentLevelUpRequirement = LevelUpInfo->LevelUpInformation[level].LevelUpRequirement;
		const int32 previousLevelUpRequirement = LevelUpInfo->LevelUpInformation[level - 1].LevelUpRequirement;

		const float deltaLevelRequirement = currentLevelUpRequirement - previousLevelUpRequirement;
		const float XPForThisLevel = NewXP - previousLevelUpRequirement;

		const float XPBarPercent = XPForThisLevel/ deltaLevelRequirement;

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent); 
	}
}


// Copyrights Skorokhod.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{ 
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	{
		if (bIsLocallyControlled)
		{
			SendMouseCursorData();
		}
		else
		{
			//We are on the server, so listen for target data.
			const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
			const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
			AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
				SpecHandle,
				ActivationPredictionKey
			).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
			const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
			if (!bCalledDelegate)
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// this line says that everything in this scope below should be predicted
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
		DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// Clear cache, when we receive the data
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
}
// Copyrights Skorokhod.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;

	Activate();
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	if(ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface) // Handle the situation when ASC not initalized yet
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, 
			[this](UAbilitySystemComponent* InASC) 
			{
				 	InASC->RegisterGameplayTagEvent(
						DebuffTag, 
						EGameplayTagEventType::NewOrRemoved
					).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
			});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = bOwnerValid && GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
	
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeathActor)
{
	Deactivate();
}

// Copyrights Skorokhod.


#include "Actor/AuraFireBall.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystem/AuraAbilitySystemBPLibrary.h>
#include <NiagaraFunctionLibrary.h>
#include <Components/AudioComponent.h>
#include <GameplayCueManager.h>
#include <AuraGameplayTags.h>

// Sets default values
AAuraFireBall::AAuraFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeline();
	
}

void AAuraFireBall::OnHit()
{
	// local gameplay cue execution

	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();

		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
} 



void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{	// on the server
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemBPLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		//Destroy();
	}
}


// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilitySystemTypes.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
protected:

	bool bHit = false;

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

protected:
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	/*
	* Functions
	*/
public:
	AAuraProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	virtual void Destroyed() override;

	bool IsValidOverlap(AActor* OtherActor) const;
	

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};




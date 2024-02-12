// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraInputConfig;
struct FGameplayTag;
class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;
	FHitResult CursorHitResult;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;


	/*
	*  Click Movement
	*/
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPRessedThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false; // if we targeting something (for example AI)

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> Spline;

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//(keyname 'struct' for forward declaration)
	void Move(const struct FInputActionValue& InputActionValue);
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC();
	
	void AutoRun();
};

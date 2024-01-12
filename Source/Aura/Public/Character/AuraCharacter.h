// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Component")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UCameraComponent* CameraComp;
	
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


private:
	void InitAbilityActorInfo();
};

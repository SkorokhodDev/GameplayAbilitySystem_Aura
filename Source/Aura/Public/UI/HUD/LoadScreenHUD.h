// Copyrights Skorokhod.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:
	// WBP_LoadMenu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ULoadScreenWidget> LoadScreenWidget;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UMVVM_LoadScreen> LoadScreenViewModel;

protected:
	virtual void BeginPlay() override;
	
};

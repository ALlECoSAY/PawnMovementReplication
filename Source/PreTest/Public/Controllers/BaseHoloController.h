#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BaseHoloController.generated.h"

class UInputAction;
class UInputMappingContext;


DECLARE_LOG_CATEGORY_EXTERN(LogBaseHoloController, Log, All);

UCLASS()
class PRETEST_API ABaseHoloController : public APlayerController
{
	GENERATED_BODY()

public:
protected:
	virtual void BeginPlay() override;

	void Look(const FInputActionValue& InputActionValue);
	virtual void SetupInputComponent() override;
	
protected:
	//input mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> LookMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
};

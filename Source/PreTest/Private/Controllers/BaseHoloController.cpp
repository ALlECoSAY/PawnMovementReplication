// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BaseHoloController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY(LogBaseHoloController);

void ABaseHoloController::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseHoloController::Look(const FInputActionValue& InputActionValue)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(855, 5.f, FColor::Red, TEXT(" Look + " + InputActionValue.Get<FVector2D>().ToString()));
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
	
}

void ABaseHoloController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(LookMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerWeaponComponent works only with UEnhancedInputLocalPlayerSubsystem"));
		return;
	}
	Subsystem->AddMappingContext(LookMappingContext, 0);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	}
}

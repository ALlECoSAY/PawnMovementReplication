// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HoloMovementComponent.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogHoloMovementComponent, Log, All);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom))
class PRETEST_API UHoloMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoloMovementComponent();

protected:
	void Move(const FInputActionValue& InputActionValue);
	
	void Jump();
	UFUNCTION(Server, Reliable)
	void Server_Jump();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Jump();

	UFUNCTION(Server, Reliable)
	void Server_AccumulateMovement(const FVector& Movement);

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float MaxSpeed = 300.f;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> MovementMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float RotationInterpSpeed = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	double JumpImpulse = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float JumpDelay = 1.f;
	
	FTimerHandle JumpTimerHandle;
};

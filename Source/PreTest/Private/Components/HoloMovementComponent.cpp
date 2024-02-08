#include "PreTest/Public/Components/HoloMovementComponent.h"

#include "DiffUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawns/HoloPawn.h"

DEFINE_LOG_CATEGORY(LogHoloMovementComponent);

UHoloMovementComponent::UHoloMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
}

void UHoloMovementComponent::Move(const FInputActionValue& InputActionValue)
{
	
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if(GEngine) GEngine->AddOnScreenDebugMessage(9992, 5.f, FColor::Red, TEXT(" Move + " + MovementVector.ToString()));
	auto* Controller = GetController();
	if (Controller != nullptr)
	{
		// find forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector InputVector = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;

		// add movement to server and autonomous proxy
		Server_AccumulateMovement(InputVector);
		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			if(GEngine) GEngine->AddOnScreenDebugMessage(33, 5.f, FColor::Red, TEXT(" AutonomousProxy + " + InputVector.ToString()));
			AddInputVector(InputVector);
		}
		
	}
	
}

void UHoloMovementComponent::Jump()
{
	//if timer still goes
	if(GetWorld()->GetTimerManager().IsTimerActive(JumpTimerHandle))
	{
		UE_LOG(LogHoloMovementComponent, Warning, TEXT("Jump delay timer is still active"));
		return;
	}
	Server_Jump_Implementation();
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpDelay, false);
}
void UHoloMovementComponent::Server_Jump_Implementation()
{
	Multicast_Jump();
}

void UHoloMovementComponent::Multicast_Jump_Implementation()
{
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	AHoloPawn* HoloPawn = CastChecked<AHoloPawn>(GetOwner());
	HoloPawn->GetCapsuleComponent()->AddImpulse(Impulse, NAME_None, true);
}

void UHoloMovementComponent::Server_AccumulateMovement_Implementation(const FVector& Movement)
{
	AddInputVector(Movement);
}

void UHoloMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	const APawn* Owner = GetOwner<APawn>();
	check(Owner);
	
	if (!Owner->IsLocallyControlled())
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Owner->Controller);
	check(PlayerController);

#pragma region Input setup

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerWeaponComponent works only with UEnhancedInputLocalPlayerSubsystem"));
		return;
	}
	
	Subsystem->AddMappingContext(MovementMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

	//Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);	
	//Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	
#pragma endregion	
}

void UHoloMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector Movement = ConsumeInputVector();
	if (Movement.IsNearlyZero())
	{
		return;
	}
	
	if (GetOwner()->HasAuthority() || (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy))
	{
		const FVector Offset = Movement * MaxSpeed * DeltaTime;
		GetOwner()->AddActorWorldOffset(Offset, false, nullptr, ETeleportType::ResetPhysics);		
	}

	const FVector MovementDirection = Movement.GetSafeNormal();
	const FRotator MovementRotation = MovementDirection.Rotation();
	const FRotator CurrentRotation = GetOwner()->GetActorRotation();
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, MovementRotation, DeltaTime, RotationInterpSpeed);
	GetOwner()->SetActorRotation(NewRotation);
}


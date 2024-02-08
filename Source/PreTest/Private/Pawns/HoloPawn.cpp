#include "PreTest/Public/Pawns/HoloPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HoloHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PreTest/Public/Components/HoloMovementComponent.h"
#include "PreTest/Public/Components/HoloMunitionComponent.h"

AHoloPawn::AHoloPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->InitCapsuleSize(50.f,100.f);
	CapsuleComponent->SetLinearDamping(20.f);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	CameraRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRootComponent"));
	CameraRootComponent->SetupAttachment(RootComponent);

	WeaponRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRootComponent"));
	WeaponRootComponent->SetupAttachment(StaticMeshComponent);
	
	CameraBoomComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComponent"));
	CameraBoomComponent->SetupAttachment(CameraRootComponent);
	CameraBoomComponent->bUsePawnControlRotation = true;
	CameraBoomComponent->TargetArmLength = 500.f;
	
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoomComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	MovementComponent = CreateDefaultSubobject<UHoloMovementComponent>(TEXT("MovementComponent"));
	MunitionComponent = CreateDefaultSubobject<UHoloMunitionComponent>(TEXT("MunitionComponent"));
	HealthComponent = CreateDefaultSubobject<UHoloHealthComponent>(TEXT("HealthComponent"));
}

void AHoloPawn::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangedDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
}

void AHoloPawn::OnHealthChanged(float CurrentHealth, float MaxHealth, AActor* Damaged)
{
	if(FMath::IsNearlyZero(CurrentHealth))
	{
		Destroy();
	}
}



UPawnMovementComponent* AHoloPawn::GetMovementComponent() const
{
	return MovementComponent.Get(); 
}

UHoloHealthComponent* AHoloPawn::GetHealthComponent() const
{
	return HealthComponent;
}


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Damagable.h"
#include "HoloPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UMovementComponent;
class UHoloMovementComponent;
class UHoloMunitionComponent;


UCLASS()
class PRETEST_API AHoloPawn : public APawn, public IDamagable
{
	GENERATED_BODY()

public:
	AHoloPawn();
	
#pragma region GET/SET

	FORCEINLINE UHoloMunitionComponent* GetMunitionComponent() const { return MunitionComponent; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	UPawnMovementComponent* GetMovementComponent() const override;

	virtual UHoloHealthComponent* GetHealthComponent() const override;
	
#pragma endregion

protected:
	virtual void BeginPlay() override;

	void OnHealthChanged(float CurrentHealth, float MaxHealth, AActor* Damaged);

public:	
	

protected:
	#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHoloMunitionComponent> MunitionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHoloMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHoloHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> CameraRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoomComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> WeaponRootComponent;

	#pragma endregion
	
};

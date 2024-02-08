#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoloHealthComponent.generated.h"

//On health updated
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, float, float, AActor*);

UCLASS(Blueprintable,BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRETEST_API UHoloHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHoloHealthComponent();

	void Heal(float HealAmount);
	void Damage(float DamageAmount);
	
protected:
	virtual void BeginPlay() override;

public:
	FOnHealthChangedSignature OnHealthChangedDelegate;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float HealthRegenAmount = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float HealthRegenRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float HealthRegenDelay = 5.f;

	bool bIsRegenerating = true;
		
};

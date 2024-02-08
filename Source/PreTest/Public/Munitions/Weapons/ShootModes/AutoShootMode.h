#pragma once

#include "CoreMinimal.h"
#include "BaseShootMode.h"
#include "AutoShootMode.generated.h"

UCLASS()
class PRETEST_API UAutoShootMode : public UBaseShootMode
{
	GENERATED_BODY()

public:
	virtual void HandleFireStart() override;
	virtual void HandleFireEnd() override;

	virtual bool IsFiring() const override;

protected:
	FTimerHandle FireHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float ShootRate = 0.1f;
};

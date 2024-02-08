#pragma once

#include "CoreMinimal.h"
#include "BaseShootMode.h"
#include "SingleShootMode.generated.h"

UCLASS()
class PRETEST_API USingleShootMode : public UBaseShootMode
{
	GENERATED_BODY()

public:
	virtual void HandleFireStart() override;
	virtual void HandleFireEnd() override;

	
protected:
	FTimerHandle DelayHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float ShootDelay = 0.3f;
};

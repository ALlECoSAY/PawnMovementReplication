#pragma once

#include "CoreMinimal.h"
#include "BaseHoloWeapon.h"
#include "TraceHoloWeapon.generated.h"

UCLASS()
class PRETEST_API ATraceHoloWeapon : public ABaseHoloWeapon
{
	GENERATED_BODY()

public:

	virtual void Fire() override;
	
	UFUNCTION(Server, Reliable)
	virtual void Server_PerformTrace();

protected:
	
	FHitResult TraceMuzzle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Config")
	float TraceDistance = 5000.f;
	
};

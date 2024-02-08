#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseShootMode.generated.h"


DECLARE_DELEGATE(FOnShouldSootSignature);


UCLASS(Blueprintable, BlueprintType, Abstract, ClassGroup = (Custom))
class PRETEST_API UBaseShootMode : public UObject
{
	GENERATED_BODY()

public:
	virtual void HandleFireStart() {}
	
	virtual void HandleFireEnd() {}

	virtual bool IsFiring() const { return false; }

public:
	FOnShouldSootSignature OnShouldShootDelegate;

};

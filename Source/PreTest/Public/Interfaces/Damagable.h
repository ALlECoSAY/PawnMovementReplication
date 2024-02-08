#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

class UHoloHealthComponent;

UINTERFACE()
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

class PRETEST_API IDamagable
{
	GENERATED_BODY()

public:

	virtual UHoloHealthComponent* GetHealthComponent() const = 0;
	
};

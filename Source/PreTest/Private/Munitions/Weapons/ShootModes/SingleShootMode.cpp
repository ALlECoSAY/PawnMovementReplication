#include "Munitions/Weapons/ShootModes/SingleShootMode.h"

void USingleShootMode::HandleFireStart()
{
	bool bIsDelayed = false;
	
	if (const UWorld* World = GetWorld())
	{
		bIsDelayed = World->GetTimerManager().IsTimerActive(DelayHandle);
	}

	if(!bIsDelayed)
	{
		OnShouldShootDelegate.ExecuteIfBound();
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, ShootDelay, false);
	}
	
}

void USingleShootMode::HandleFireEnd()
{
	
}

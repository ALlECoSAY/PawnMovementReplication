#include "Components/HoloHealthComponent.h"


UHoloHealthComponent::UHoloHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHoloHealthComponent::Heal(float HealAmount)
{
	const float CashedHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	OnHealthChangedDelegate.Broadcast(CurrentHealth, CashedHealth - CurrentHealth, GetOwner());
}

void UHoloHealthComponent::Damage(float DamageAmount)
{
	const float CashedHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	OnHealthChangedDelegate.Broadcast(CurrentHealth, CashedHealth - CurrentHealth, GetOwner());
}

void UHoloHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		bIsRegenerating = true;
		//todo
	}
	
}

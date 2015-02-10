

#include "Caede.h"
#include "VitalityComponent.h"


UVitalityComponent::UVitalityComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	HasHealth = true;
	HasStamina = true;
	HasStarvation = true;
	HasThirst = true;
	HasUrin = true;
	HasShit = true;
	HasAlcohol = true;
	HasPoison = true;
	HasDrug = true;

	//////////////////////////////////////////////////////////////////////////

	MaxHealth = 100;
	MaxStamina = 100;

	MaxStarvation = 100;
	MaxThirst = 100;
	MaxUrin = 100;
	MaxShit = 100;

	MaxAlcohol = 100;
	MaxPoison = 100;
	MaxDrug = 100;

	//////////////////////////////////////////////////////////////////////////

	Health = MaxHealth;
	Stamina = MaxStamina;

	Starvation = 0;
	Thirst = 0;
	Urin = 0;
	Shit = 0;

	Alcohol = 0;
	Poison = 0;
	Drug = 0;


	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

//////////////////////////////////////////////////////////////////////////

void UVitalityComponent::DoRegenerateHealth(float Amount)
{
	if (HasHealth && Amount >= 0 && IsAlive() && Health < MaxHealth)
	{
		Health += Amount;
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
	}
}

void UVitalityComponent::DoLooseHealth(float Amount)
{
	if (HasHealth && Amount >= 0 && IsAlive())
	{
		Health -= Amount;
		if (Health <= 0)
		{
			Health = 0;
			OnCharacterDeath.Broadcast(this->OwningCharacter);
		}
	}
}

void UVitalityComponent::DoRegenerateStamina(float Amount)
{
	if (HasStamina && Amount >= 0 && IsAlive() && Stamina < MaxStamina)
	{
		Stamina += Amount;
		if (Stamina > MaxStamina)
		{
			Stamina = MaxStamina;
		}
	}
}

void UVitalityComponent::DoLooseStamina(float Amount)
{
	if (HasStamina && Amount >= 0 && IsAlive() && Stamina > 0)
	{
		Stamina -= Amount;
		if (Stamina <= 0)
		{
			Stamina = 0;
			OnCharacterExausted.Broadcast(this->OwningCharacter);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void UVitalityComponent::DoEat(float Amount)
{
	if (HasStarvation && Amount >= 0 && IsAlive() && Starvation > 0)
	{
		Starvation -= Amount;
		if (Starvation < 0)
		{
			Starvation = 0;
		}
	}
}

void UVitalityComponent::DoDrink(float Amount)
{
	if (HasThirst && Amount >= 0 && IsAlive() && Thirst > 0)
	{
		Thirst -= Amount;
		if (Thirst < 0)
		{
			Thirst = 0;
		}
	}
}

void UVitalityComponent::DoUrinate(float Amount)
{
	if (HasUrin && Amount >= 0 && IsAlive() && Urin > 0)
	{
		Urin -= Amount;
		if (Urin < 0)
		{
			Urin = 0;
		}
	}
}

void UVitalityComponent::DoShit(float Amount)
{
	if (HasShit && Amount >= 0 && IsAlive() && Shit > 0)
	{
		Shit -= Amount;
		if (Shit < 0)
		{
			Shit = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void UVitalityComponent::DoConsumeAlcohol(float Amount)
{
	if (HasAlcohol && Amount >= 0 && IsAlive() && Alcohol < MaxAlcohol)
	{
		Alcohol += Amount;
		if (Alcohol > MaxAlcohol)
		{
			Alcohol = MaxAlcohol;
		}
	}
}

void UVitalityComponent::DoConsumePoison(float Amount)
{
	if (HasPoison && Amount >= 0 && IsAlive() && Poison < MaxPoison)
	{
		Poison += Amount;
		if (Poison > MaxPoison)
		{
			Poison = MaxPoison;
		}
	}
}

void UVitalityComponent::DoConsumeDrug(float Amount)
{
	if (HasDrug && Amount >= 0 && IsAlive() && Drug < MaxDrug)
	{
		Drug += Amount;
		if (Drug > MaxDrug)
		{
			Drug = MaxDrug;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

bool UVitalityComponent::IsAlive()
{
	return Health > 0;
}

bool UVitalityComponent::Revive()
{
	if (CanRevive)
	{
		MaxHealth = 100;
		MaxStamina = 100;

		MaxStarvation = 100;
		MaxThirst = 100;
		MaxUrin = 100;
		MaxShit = 100;

		MaxAlcohol = 100;
		MaxPoison = 100;
		MaxDrug = 100;


		Health = MaxHealth;
		Stamina = MaxStamina;

		Starvation = 0;
		Thirst = 0;
		Urin = 0;
		Shit = 0;

		Alcohol = 0;
		Poison = 0;
		Drug = 0;

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool UVitalityComponent::CheckForLowerLimit(TEnumAsByte<EChacracterStatus::Type> StatusType, float Limit)
{
	switch (StatusType)
	{
	case EChacracterStatus::Health:
		if (Health < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Stamina:
		if (Stamina < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Starvation:
		if (Starvation < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Thirst:
		if (Thirst < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Shit:
		if (Shit < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Urin:
		if (Urin < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Alcohol:
		if (Alcohol < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Poison:
		if (Poison < Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Drug:
		if (Drug < Limit)
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool UVitalityComponent::CheckForGreaterLimit(TEnumAsByte<EChacracterStatus::Type> StatusType, float Limit)
{
	switch (StatusType)
	{
	case EChacracterStatus::Health:
		if (Health > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Stamina:
		if (Stamina > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Starvation:
		if (Starvation > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Thirst:
		if (Thirst > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Shit:
		if (Shit > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Urin:
		if (Urin > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Alcohol:
		if (Alcohol > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Poison:
		if (Poison > Limit)
		{
			return true;
		}
		break;
	case EChacracterStatus::Drug:
		if (Drug > Limit)
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

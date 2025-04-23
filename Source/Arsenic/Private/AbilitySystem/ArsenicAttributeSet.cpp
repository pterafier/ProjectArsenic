// Copyright Silvergrove Studio


#include "AbilitySystem/ArsenicAttributeSet.h"

#include "GameplayEffectExtension.h"

UArsenicAttributeSet::UArsenicAttributeSet()
{
	// Attributes initialized via gameplay effects on the player character class
}

void UArsenicAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// Vital Attributes
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
	// Primary Attributes
	if (Attribute == GetEsotericAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEsoteric());
	}
	if (Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStrength());
	}
	if (Attribute == GetPerceptionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxPerception());
	}
	if (Attribute == GetResolveAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxResolve());
	}

	// Movement Attributes
	if (Attribute == GetWalkSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxWalkSpeed());
	}
	if (Attribute == GetSprintSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSprintSpeed());
	}
	
	// Base Attributes
	if (Attribute == GetBaseEsotericAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEsoteric());
	}
	if (Attribute == GetBaseStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStrength());
	}
	if (Attribute == GetBasePerceptionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxPerception());
	}
	if (Attribute == GetBaseResolveAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxResolve());
	}
	if (Attribute == GetBaseWalkSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxWalkSpeed());
	}
	if (Attribute == GetBaseSprintSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSprintSpeed());
	}
}

void UArsenicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Vital Attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	// Primary Attributes
	if (Data.EvaluatedData.Attribute == GetEsotericAttribute())
	{
		SetEsoteric(FMath::Clamp(GetEsoteric(), 0.f, GetMaxEsoteric()));
	}
	if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
	{
		SetStrength(FMath::Clamp(GetStrength(), 0.f, GetMaxStrength()));
	}
	if (Data.EvaluatedData.Attribute == GetPerceptionAttribute())
	{
		SetPerception(FMath::Clamp(GetPerception(), 0.f, GetMaxPerception()));
	}
	if (Data.EvaluatedData.Attribute == GetResolveAttribute())
	{
		SetResolve(FMath::Clamp(GetResolve(), 0.f, GetMaxResolve()));
	}

	// Movement Attributes
	if (Data.EvaluatedData.Attribute == GetWalkSpeedAttribute())
	{
		SetWalkSpeed(FMath::Clamp(GetWalkSpeed(), 0.f, GetMaxWalkSpeed()));
	}
	if (Data.EvaluatedData.Attribute == GetSprintSpeedAttribute())
	{
		SetSprintSpeed(FMath::Clamp(GetSprintSpeed(), 0.f, GetMaxSprintSpeed()));
	}

	// Base Attributes
	if (Data.EvaluatedData.Attribute == GetBaseEsotericAttribute())
	{
		SetBaseEsoteric(FMath::Clamp(GetBaseEsoteric(), 0.f, GetMaxEsoteric()));
	}
	if (Data.EvaluatedData.Attribute == GetBaseStrengthAttribute())
	{
		SetBaseStrength(FMath::Clamp(GetBaseStrength(), 0.f, GetMaxStrength()));
	}
	if (Data.EvaluatedData.Attribute == GetBasePerceptionAttribute())
	{
		SetBasePerception(FMath::Clamp(GetBasePerception(), 0.f, GetMaxPerception()));
	}
	if (Data.EvaluatedData.Attribute == GetBaseResolveAttribute())
	{
		SetBaseResolve(FMath::Clamp(GetBaseResolve(), 0.f, GetMaxResolve()));
	}
	if (Data.EvaluatedData.Attribute == GetBaseWalkSpeedAttribute())
	{
		SetBaseWalkSpeed(FMath::Clamp(GetBaseWalkSpeed(), 0.f, GetMaxWalkSpeed()));
	}
	if (Data.EvaluatedData.Attribute == GetBaseSprintSpeedAttribute())
	{
		SetBaseSprintSpeed(FMath::Clamp(GetBaseSprintSpeed(), 0.f, GetMaxSprintSpeed()));
	}
}

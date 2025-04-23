// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ArsenicAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class ARSENIC_API UArsenicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UArsenicAttributeSet();
	
/**
 * Vital Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Health);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Mana);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxMana);

/**
 * Primary Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData Esoteric;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Esoteric);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData MaxEsoteric;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxEsoteric);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Strength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxStrength);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData Perception;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Perception);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData MaxPerception;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxPerception);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData Resolve;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, Resolve);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData MaxResolve;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxResolve);
	
/**
 * Secondary Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, HealthRegen);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, ManaRegen);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, PhysicalResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MagicResistance);

/**
 * Movement Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, WalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxWalkSpeed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, SprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData MaxSprintSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, MaxSprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData LookSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, LookSpeed);

/**
 * Base Attributes
 *
 * We will use these in case there's a temporary buff to a stat, we can always return to the base value
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxHealth;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseMaxHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxMana;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseMaxMana);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseEsoteric;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseEsoteric);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseStrength;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseStrength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BasePerception;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BasePerception);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseResolve;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseResolve);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData BaseWalkSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseWalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData BaseSprintSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseSprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData BaseLookSpeed;
	ATTRIBUTE_ACCESSORS(UArsenicAttributeSet, BaseLookSpeed);
	
public:
	// Used to clamp internal values of attributes
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Used to clamp attribute values for display purposes
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	
	
};

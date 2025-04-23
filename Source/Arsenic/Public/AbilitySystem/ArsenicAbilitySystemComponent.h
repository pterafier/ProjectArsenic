// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ArsenicAbilitySystemComponent.generated.h"

/* Dynamic and native versions of the same delegates so we can use Lambda convenience but allow a blueprint callable delegate */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicEffectAssetTags, const FGameplayTagContainer&, AssetTags);
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

/**
 * 
 */
UCLASS()
class ARSENIC_API UArsenicAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();
	void AddStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Applies a gameplay effect to the target Ability System Component - can use Self as target
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, float EffectLevel);
	
/**
 * Delegates
 */
	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FDynamicEffectAssetTags OnEffectAssetTagsChanged;

	FEffectAssetTags EffectAssetTags;

protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};

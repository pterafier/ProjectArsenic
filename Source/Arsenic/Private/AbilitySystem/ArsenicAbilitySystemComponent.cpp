// Copyright Silvergrove Studio


#include "AbilitySystem/ArsenicAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/ArsenicGameplayAbilityBase.h"

void UArsenicAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied); // bind callback function for responding to effects when they're applied
}

void UArsenicAbilitySystemComponent::AddStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UArsenicGameplayAbilityBase* Ability = Cast<UArsenicGameplayAbilityBase>(AbilitySpec.Ability))
		{
			GiveAbility(AbilitySpec);
		}
	}
}

void UArsenicAbilitySystemComponent::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, float EffectLevel)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContextHandle);
	ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
}

void UArsenicAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer; // Create a tag container
	EffectSpec.GetAllAssetTags(TagContainer); // Get all tags. Can use asset tags, granted tags, etc. Many container types to choose from but this is all tags that are applied to the effect
	
	EffectAssetTags.Broadcast(TagContainer);
	OnEffectAssetTagsChanged.Broadcast(TagContainer); // can bind to this in Blueprint to display UI elements based on a gameplay tag when an effect is applied
	
	for (FGameplayTag Tag : TagContainer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("UArsenicAbilitySystemComponent: %s effect applied!"), *Tag.ToString()));
	}
}

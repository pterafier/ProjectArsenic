// Copyright Silvergrove Studio


#include "Player/ArsenicPlayerState.h"

#include "AbilitySystem/ArsenicAbilitySystemComponent.h"
#include "AbilitySystem/ArsenicAttributeSet.h"
#include "Characters/ArsenicPlayerCharacter.h"
#include "Game/Save/ArsenicSaveGameSubsystem.h"
#include "Game/Save/ArsenicSaveGameObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AArsenicPlayerState::AArsenicPlayerState()
{
	ArsenicAbilitySystemComponent = CreateDefaultSubobject<UArsenicAbilitySystemComponent>("Arsenic Ability System Component");

	ArsenicAttributeSet = CreateDefaultSubobject<UArsenicAttributeSet>("Arsenic Attribute Set");
	
	LoadPlayerDataAtBeginPlay = true;
	BoundToDelegate = false;
}

FPlayerSave AArsenicPlayerState::GetPlayerSaveData()
{
	FPlayerSave PlayerData;
	PlayerData.PlayerTransform = ArsenicPlayerCharacter->GetTransform();
	PlayerData.PlayerControlRotation = ArsenicPlayerCharacter->GetControlRotation();

	return PlayerData;
}

void AArsenicPlayerState::SetPlayerStateFromSaveData()
{
	UArsenicSaveGameSubsystem* SaveSubsystem = UArsenicSaveGameSubsystem::Get(this);
	UArsenicSaveGameObject* SaveGameObject = IArsenicSaveGameInterface::Execute_GetSaveGameData(SaveSubsystem);

	if (SaveSubsystem->DoesSaveExist)
	{
		if (SaveSubsystem->SaveObjectInitialized)
		{
			FPlayerSave PlayerData = SaveGameObject->PlayerData;
			ArsenicPlayerCharacter->SetActorTransform(PlayerData.PlayerTransform);
			ArsenicPlayerCharacter->ArsenicPlayerController->SetControlRotation(PlayerData.PlayerControlRotation);
			
			InitializeAttributes(SaveGameObject->PlayerAttributes);
		}
		else
		{
			if (BoundToDelegate) return;
			SaveSubsystem->OnSaveGameObjectLoaded.AddDynamic(this, &ThisClass::OnSaveGameObjectLoaded_Callback);
			BoundToDelegate = true;
		}
	}
	else
	{
		FCharacterAttributes NewPlayerAttributes;
		InitializeAttributes(NewPlayerAttributes);
	}
}

void AArsenicPlayerState::OnSaveGameObjectLoaded_Callback()
{
	SetPlayerStateFromSaveData();
}

UAbilitySystemComponent* AArsenicPlayerState::GetAbilitySystemComponent() const
{
	return ArsenicAbilitySystemComponent;
}

void AArsenicPlayerState::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
	BindAttributeDelegates();
	if (LoadPlayerDataAtBeginPlay)
	{
		SetPlayerStateFromSaveData();
	}
}

void AArsenicPlayerState::InitializeAttributes(const FCharacterAttributes& PlayerAttributes)
{
	check(ArsenicAbilitySystemComponent);
	check(InitializeAttributesEffect);

	FGameplayEffectSpecHandle EffectSpecHandle = ArsenicAbilitySystemComponent->MakeOutgoingSpec(
		InitializeAttributesEffect,
		1.0f,
		ArsenicAbilitySystemComponent->MakeEffectContext()
	);
	
	FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxHealth")), PlayerAttributes.MaxHealth);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Health")), PlayerAttributes.Health);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxMana")), PlayerAttributes.MaxMana);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Mana")), PlayerAttributes.Mana);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Esoteric")), PlayerAttributes.Esoteric);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxEsoteric")), PlayerAttributes.MaxEsoteric);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Strength")), PlayerAttributes.Strength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxStrength")), PlayerAttributes.MaxStrength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Perception")), PlayerAttributes.Perception);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxPerception")), PlayerAttributes.MaxPerception);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Resolve")), PlayerAttributes.Resolve);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxResolve")), PlayerAttributes.MaxResolve);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.HealthRegen")), PlayerAttributes.HealthRegen);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.ManaRegen")), PlayerAttributes.ManaRegen);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.PhysicalResistance")), PlayerAttributes.PhysicalResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MagicResistance")), PlayerAttributes.MagicResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.WalkSpeed")), PlayerAttributes.WalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxWalkSpeed")), PlayerAttributes.MaxWalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.SprintSpeed")), PlayerAttributes.SprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxSprintSpeed")), PlayerAttributes.MaxSprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.LookSpeed")), PlayerAttributes.LookSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxHealth")), PlayerAttributes.BaseMaxHealth);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxMana")), PlayerAttributes.BaseMaxMana);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseEsoteric")), PlayerAttributes.BaseEsoteric);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseStrength")), PlayerAttributes.BaseStrength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BasePerception")), PlayerAttributes.BasePerception);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseResolve")), PlayerAttributes.BaseResolve);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseWalkSpeed")), PlayerAttributes.BaseWalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseSprintSpeed")), PlayerAttributes.BaseSprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseLookSpeed")), PlayerAttributes.BaseLookSpeed);
	
	ArsenicAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec);
}

void AArsenicPlayerState::SetReferences()
{
	ArsenicPlayerCharacter = Cast<AArsenicPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	ArsenicPlayerController = Cast<AArsenicPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void AArsenicPlayerState::BindAttributeDelegates()
{
	check(ArsenicAbilitySystemComponent)
	
	ArsenicAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UArsenicAttributeSet::GetWalkSpeedAttribute()).AddUObject(this, &ThisClass::OnWalkSpeedChanged_Callback);
	ArsenicAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UArsenicAttributeSet::GetLookSpeedAttribute()).AddUObject(this, &ThisClass::OnLookSpeedChanged_Callback);
}

void AArsenicPlayerState::OnWalkSpeedChanged_Callback(const FOnAttributeChangeData& Data)
{
	ArsenicPlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AArsenicPlayerState::OnLookSpeedChanged_Callback(const FOnAttributeChangeData& Data)
{
	ArsenicPlayerController->BaseTurnRate = Data.NewValue;
}

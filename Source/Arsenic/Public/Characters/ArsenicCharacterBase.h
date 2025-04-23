// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ArsenicCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/*
* The ability system component pointers are setup on ArsenicCharacterBase. This means that Arsenic and all enemies will have pointers to the ability system component. HOWEVER, Arsenic's ACTUAL ability system component and attributes
* are set up and initialized on the PlayerState.
* 
* We included the IAbilitySystemInterface to use the getter for the ability system component. We have done the same thing for GetAttributeSet() except it's not from an interface. 
* This also should be done on the base class so all characters in the world can return the ASC. It needed to be added to this class because Arsenic's ASC lives on the PlayerState
* 
* For the setup of future ability system components, inherit from the ability system interface and create the attributeset getter function, unless you're inheriting from ArsenicCharacterBase, in which case
* just override the functions that live there
*
* The ability system component has 2 variables for setting owners - the OwnerActor and AvatarActor. If the ability system component is on the character, then the AvatarActor is also the OwnerActor.
* But if it's on the PlayerState (Or another entity) the OwnerActor is the PlayerState, but the AvatarActor is character. Set with InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
* The ability system component needs to be initialized with these actors - more info https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39782798#questions
* InitAbilityActorInfo must be called after possession (the Controller has been set for the Pawn)
* If the ASC lives on the pawn, call InitAbilityActorInfo in PossessedBy() on the server, and AcknowledgePossession() on the Client
* If the ASC lives on the PlayerState, call InitAbilityActorInfo in PossessedBy() on the Server, and OnRep_PlayerState on the Client
* If the ASC lives on the pawn and is AI-Controlled, simply call InitAbilityActorInfo on BeginPlay()
* 
* Console command showdebug abilitysystem shows debug info about the ability system
*/

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = MaxMana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Esoteric = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxEsoteric = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStrength = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Perception = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPerception = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resolve = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxResolve = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegen = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaRegen = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicalResistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicResistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LookSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxHealth = MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxMana = MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseEsoteric = Esoteric;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseStrength = Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasePerception = Perception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseResolve = Resolve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWalkSpeed = WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSprintSpeed = SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseLookSpeed = LookSpeed;
};

UCLASS()
class ARSENIC_API AArsenicCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArsenicCharacterBase();
	
	/** Begin AbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End AbilitySystemInterface */
	virtual UAttributeSet* GetAttributeSet() const { return ArsenicAttributeSet; };

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ArsenicAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeSet> ArsenicAttributeSet;
	
};

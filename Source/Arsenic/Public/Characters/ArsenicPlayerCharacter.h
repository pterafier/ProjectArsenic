// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Characters/ArsenicCharacterBase.h"
#include "ArsenicPlayerCharacter.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class AArsenicPlayerState;
class AArsenicPlayerController;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLookAt, AActor*, Actor);

/**
 * 
 */
UCLASS()
class ARSENIC_API AArsenicPlayerCharacter : public AArsenicCharacterBase
{
	GENERATED_BODY()

public:
	AArsenicPlayerCharacter();
	
/*
 * Public Variables
 */
	// The main camera for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerController> ArsenicPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerState> ArsenicPlayerState;

	// An array of abilities to grant to the player when the game starts
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> LookAtActor;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPlayerLookAt OnPlayerLookAt;

/*
 * Public Methods
 */
	// Called when the character gets possessed by the controller
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Save")
	FCharacterAttributes GetAttributesSaveData();

	UFUNCTION(BlueprintCallable)
	void Interact();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	void AddStartupAbilities();

private:
	virtual void InitAbilityActorInfo() override;

	// Function run on tick to detect interactable objects
	void InteractTrace();

	// Helper function to clean up Interact Trace
	AActor* PerformInteractLineTrace();

	// Organizational method to reduce lines of code on PossessedBy. Just sets references to necessary actors in the game
	void SetPlayerReferences(AController* NewController);
};

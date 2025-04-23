// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArsenicPlayerController.generated.h"

class UCommonActivatableWidget;
class UMainWidget;
class APlayerHUD;
class UActivatableWidgetBase;
class AArsenicPlayerCharacter;
struct FInputActionValue;
class UInputMappingContext;
class AArsenicPlayerState;
class UInputAction;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftTabPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightTabPressed);
/**
 * 
 */

// Player Controls
USTRUCT(BlueprintType)
struct FControlsSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	bool InvertY;

	FControlsSave() :
		BaseTurnRate(0.5f),
		InvertY(false)
	{}
};


UCLASS()
class ARSENIC_API AArsenicPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
/*
 * Public Variables
 */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TabLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TabRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool InvertY;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	APawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerState> ArsenicPlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerCharacter> ArsenicPlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UMainWidget> ArsenicMainWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> PauseMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCommonActivatableWidget> PauseMenuInstance;

	// This gets the ACTOR HUD which is set in the game mode, not the HUD that has progress bars and other information.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<APlayerHUD> PlayerHUDActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI");
	bool isMenuActivated;

	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnLeftTabPressed OnLeftTabPressed;

	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnRightTabPressed OnRightTabPressed;
	
/*
 * Public Methods
 */
	
	//Constructor
	AArsenicPlayerController();

	// Called from the PlayerHUDActor when it's initialized to inform the player controller that it's safe to do widget stuff
	void HUDInitialized();

	// Called when menus are closed to restore gameplay input mapping context
	void SwitchToGameplayInput() const;

	// Called when menus are opened to use menu input mapping context
	void SwitchToMenuInput() const;

	// These methods are called when the player tries to tab a menu left or right. They must have the correct IMC assigned
	void OnTabLeftPressed();
	void OnTabRightPressed();

protected:
/*
 * Protected Methods
 */
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; // Overridden to create a reference to the player state at the earliest possible time
	
	// APawn interface
	void SetupInputComponent(UInputComponent* InputComponent);
	// End of APawn interface

	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = Input)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable, Category = Input)
	void Look(const FInputActionValue& Value);

	/** Called for opening and closing the pause menu */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandlePauseMenu();

	// Responsible for setting menu bools back to false. See HandlePauseMenu for example
	UFUNCTION(Category = "UI")
	void OnStacksCleared();

	UFUNCTION()
	void CallPlayerInteract();

/*
 * Protected Variables
 */
	
	/** Input Mapping Context to be used for player input during gameplay not involving menus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* GameInputMappingContext;

	/** Input Mapping Context to be used for player input during gameplay only involving menus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MenuInputMappingContext;

private:
	void SetReferences();
	
	
};

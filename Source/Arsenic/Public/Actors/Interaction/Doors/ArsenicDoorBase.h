// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interaction/ArsenicInteractActorBase.h"
#include "ArsenicDoorBase.generated.h"

class UArrowComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	SingleSliding		UMETA(DisplayName = "Single Sliding"),
	Hinged				UMETA(DisplayName = "Hinged"),
	DoubleSliding		UMETA(DisplayName = "Double Sliding")
};

USTRUCT(BlueprintType)
struct FDoorSave : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStatInteractionSuccessful = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDoorLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpened = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartingRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingForwardVector = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SecondDoorStartingLocation = FVector::ZeroVector;
};

/**
 * 
 */
UCLASS()
class ARSENIC_API AArsenicDoorBase : public AArsenicInteractActorBase
{
	GENERATED_BODY()

public:
	AArsenicDoorBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> Arrow;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Properties")
	EDoorType DoorType;

	// MUST BE UNIQUE - helps manage the doors' states as they load and unload
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FName UniqueDoorName;

	// Set this to true if this door requires a stat check to open. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	bool RequiresStatCheck;

	// Used for sliding doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (MakeEditWidget = true, EditCondition = "DoorType != EDoorType::Hinged", EditConditionHides))
	FVector OpenPoint1;

	// Set to true if you want the door to be opened at begin play. Won't work if CloseDoorAutomatically is true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (EditCondition = "DoorType == EDoorType::Hinged && RequiresStatCheck == false", EditConditionHides))
	bool StartOpened;

	// Set to true if you want the door to close automatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (EditCondition = "DoorType == EDoorType::Hinged", EditConditionHides))
	bool CloseDoorAutomatically;

	// Determines how far away the player needs to be before automatically closing. Must be a larger number than DistanceToOpen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (EditCondition = "CloseDoorAutomatically == true", EditConditionHides))
	float DistanceToClose;

	// Determines how close the player needs to be before the door automatically opens. Must be a smaller number than DistanceToClose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (EditCondition = "DoorType != EDoorType::Hinged", EditConditionHides))
	float DistanceToOpen;

	// Determines how quickly the door opens and closes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DefaultDoorSpeed;

	// Multiplied by the DefaultDoorSpeed when slamming the door closed (slamming occurs when the door becomes locked and the door is open)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float SlamSpeedMultiplier;

	// Calculated every second (or half-second for sliding doors) and utilized by various door systems for performance purposes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc")
	float DistanceToPlayer;

	// True when the door is slammed closed
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	bool DoorSlammed;

	// True when the door is locked
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	bool DoorLocked;

	// True when the door either doesn't require a stat check to open, or when the stat check is completed successfully
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool StatInteractionSuccessful;

/*
 *	Public Methods
 */
	// Tries closing the door based on selected door type
	UFUNCTION(BlueprintCallable, Category = "Door State")
	void TryCloseDoor();

	// Tries opening the door based on selected door type
	UFUNCTION(BlueprintCallable, Category = "Door State")
	void TryOpenDoor();

	// Tries slamming the door based on selected door type. Simply closes much faster
	UFUNCTION(BlueprintCallable, Category = "Door State")
	void TrySlamDoor();

	// Closes and locks the door
	UFUNCTION(BlueprintCallable, Category = "Door State")
	void LockDoor();

	// Unlocks the door
	UFUNCTION(BlueprintCallable, Category = "Door State")
	void UnlockDoor();

	// Populates and returns an FDoorSave struct with this door's current state data
	UFUNCTION(BlueprintCallable, Category = "Save")
	FDoorSave& GetDoorSaveData();

	// Updates this door's entry in the CollectedDoorData map on the SaveSubsystem
	UFUNCTION(BlueprintCallable, Category = "Save")
	void UpdateDoorMap();

	// Finds this door's entry in the CollectedDoorData map on the SaveSubsystem, then sets its state with that data
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetDoorStateFromSaveData(FDoorSave& FoundData);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorLocked();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorUnlocked();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorStartOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorStartClose();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorStartSlam();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorFinishOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorFinishClose();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActionTextChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorStateLoaded();

	UFUNCTION()
	void OnSaveGameObjectLoaded_Callback();

	/** Begin Interact Interface */
	virtual void Interact_Implementation() override;
	virtual void LookAt_Implementation() override;
	/** End Interact Interface */

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void CacheStartingPositions();
	virtual void CloseDoubleSlidingDoor();
	virtual void OpenDoubleSlidingDoor();

	FRotator StartingRotation;
	FVector StartingForwardVector;
	FVector StartingLocation;
	FRotator DoorRotationDirection;
	
	float DoorSpeed;
	float DistanceCheckInterval;
	float SlidingDoorDistanceCheckInterval;
	float HingedDoorDistanceCheckInterval;
	bool OpenAnimationStarted;
	bool ClosedAnimationStarted;
	bool DoorOpen;
	bool BoundToOnGameLoadedDelegate;

	FTimerHandle DistanceToPlayerTimerHandle;
	FTimerHandle DoorAnimationTimerHandle;

private:
	FRotator CalculateHingedDoorOpenDirection();
	FDoorSave DoorSaveData;
	void InterpToHingedDoorOpenPosition();
	void InterpToSlidingDoorOpenPosition();
	void CheckPlayerDistance();
	void CloseSlidingDoor();
	void CloseHingedDoor();
	void OpenSlidingDoor();
	void OpenHingedDoor();
	void InterpToHingedDoorClosedPosition();
	void InterpToSlidingDoorClosedPosition();
	void SetStateOpened_Hinged();
	void SetStateClosed_Hinged();
	void InitializeFirstTimeDoorState();
	void StartDistanceCheckTimer();
	void RegisterWithSaveSubsystem();
};



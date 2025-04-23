// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PylonManager.generated.h"

USTRUCT(BlueprintType)
struct FPylonStateSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pylon")
	int32 CurrentActivePylons = 3;

	FPylonStateSave() {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPylonStateChanged, int32, ActivePylons);
/**
 * 
 */
UCLASS()
class ARSENIC_API UPylonManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	public:
	// Returns the instance of the PylonManager
	static UPylonManager* Get(const UObject* WorldContextObject);

	//Initializes the subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//Gets the number of active pylons
	UFUNCTION(BlueprintCallable, Category = "Pylon")
	int32 GetActivePylons() const {return ActivePylons; }

	//Decreases the number of active pylons and triggers the event
	UFUNCTION(BlueprintCallable, Category = "Pylon")
	void RemovePylon();

	//Delegate that broadcasts when pylons are down
	UPROPERTY(BlueprintAssignable, Category = "Pylon")
	FOnPylonStateChanged OnPylonStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Save")
	FPylonStateSave GetPylonSaveData() const;

	private:
	//Tracks the number of active pylons
	int32 ActivePylons = 3;
};

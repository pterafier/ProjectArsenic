// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interaction/Doors/ArsenicDoorBase.h"
#include "Characters/ArsenicCharacterBase.h"
#include "Game/Save/ArsenicSaveGameInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArsenicSaveGameSubsystem.generated.h"

class UCommonActivatableWidget;
class UActivatableWidgetBase;
class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameObjectLoadedDelegate);
/**
 * In a perfect world this class would be renamed to USaveGameSubsystem, but I'd really rather not break the entire project for a rename
 */
UCLASS(Config = Game)
class ARSENIC_API UArsenicSaveGameSubsystem : public UGameInstanceSubsystem, public IArsenicSaveGameInterface
{
	GENERATED_BODY()

public:
	UArsenicSaveGameSubsystem();

/*
 * Variables
 */
	// The object that stores the actual data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	TObjectPtr<UArsenicSaveGameObject> ArsenicSaveGameObject;

	// The save name of the slot we want to save to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FString SaveSlotName;

	// Useful if we need to change save game objects, otherwise this will always be set to UArsenicSaveGameObject
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	TSubclassOf<USaveGame> SaveObjectClass;
	
	// Used to dictate the state of various buttons in the UI and objects in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool DoesSaveExist;

	// Used to check that the save object has been properly initialized
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool SaveObjectInitialized;

	// Helper bool to allow checkpoints to know when they can and cannot save based on the state of the save game object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	bool CanSave;

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	TSubclassOf<UActivatableWidgetBase> SaveIndicatorWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	TObjectPtr<UCommonActivatableWidget> SaveIndicatorWidget;

	UPROPERTY()
	FOnSaveGameObjectLoadedDelegate OnSaveGameObjectLoaded;

	// Helps track the state of all doors in the world as their state changes
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	TMap<FName, FDoorSave> CollectedDoorData;

/*
 * Methods
 */
	// Returns the instance of the subsystem
	static UArsenicSaveGameSubsystem* Get(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetSaveIndicatorWidgetClass(TSubclassOf<UActivatableWidgetBase> WidgetClass);

	// Doors call this function as their states change so when the player saves, all the doors (loaded or not) get properly saved. 
	UFUNCTION(BlueprintCallable, Category = "Save")
	void UpdateCollectedDoorData(FName DoorName, FDoorSave& DoorData);

	// override the subsystem's Initialize function
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void SetGraphicsSettingsFromSaveData();

	void SetResolution();

	// Begin IArsenicSaveGameInterface
	virtual UArsenicSaveGameObject* GetSaveGameData_Implementation() const override;
	virtual void LoadGameData_Implementation() override;
	virtual void SaveGameData_Implementation() override;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) override;
	virtual void SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes) override;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& ControlsData) override;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData) override;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) override;
	virtual void SetPylonSaveData_Implementation(const FPylonStateSave& PylonData) override;
	virtual void SetDoorSaveData_Implementation(const TMap<FName, FDoorSave>& DoorData) override;
	// End interface

protected:
	
	UFUNCTION()
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	UFUNCTION()
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGameObject);

	void PushSaveIndicatorWidget();
};

// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArsenicSaveGameInterface.generated.h"

struct FCharacterAttributes;
struct FGraphicsSave;
struct FControlsSave;
struct FAudioSave;
struct FPlayerSave;
struct FPylonStateSave;
struct FDoorSave;
class UArsenicSaveGameObject;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UArsenicSaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARSENIC_API IArsenicSaveGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	// I want these functions to also have blueprint support, so I need to create 2 versions. To call these functions in blueprint
	// I'll need to simply call GetSaveGameData(), or LoadGameData() - I won't have to worry about the _Implementation versions.
	// But if I want to use these functions in C++, I'll have to override the _Implementation versions.
	// Using "= 0" at the end of the _Implementation versions REQUIRES any classes that implement this interface to also implement those functions.
	// If they don't, the project won't build. If I want to override those functions in JamPlayerCharacter (which I definitely do), it would look like this:
	// virtual void LoadGameData_Implementation() override;
	// Then i can simply write whatever code I want in the .cpp file of that class
	
public:
	// Function used to return save data from the class that implements this interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	UArsenicSaveGameObject* GetSaveGameData() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void LoadGameData();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SaveGameData();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPlayerSaveData(const FPlayerSave& PlayerData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPlayerAttributes(const FCharacterAttributes& PlayerAttributes);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetControlsSaveData(const FControlsSave& SavedControlsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetGraphicsSaveData(const FGraphicsSave& GraphicsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetAudioSettingsSaveData(const FAudioSave& AudioData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPylonSaveData(const FPylonStateSave& PylonData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetDoorSaveData(const TMap<FName, FDoorSave>& DoorData);

	virtual UArsenicSaveGameObject* GetSaveGameData_Implementation() const = 0;
	virtual void LoadGameData_Implementation() = 0;
	virtual void SaveGameData_Implementation() = 0;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) = 0;
	virtual void SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes) = 0;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& ControlsData) = 0;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData) = 0;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) = 0;
	virtual void SetPylonSaveData_Implementation(const FPylonStateSave& PylonData) = 0;
	virtual void SetDoorSaveData_Implementation(const TMap<FName, FDoorSave>& DoorData) = 0; // can't pass FDoorSave by reference because TMaps need actual values, not references
};

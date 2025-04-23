// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Data/GeneralSettingsSaveData.h"
#include "GameFramework/SaveGame.h"
#include "Player/ArsenicPlayerController.h"
#include "Player/ArsenicPlayerState.h"
#include "Characters/ArsenicCharacterBase.h"
#include "Game/Systems/PylonManager.h"
#include "Actors/Interaction/Doors/ArsenicDoorBase.h"
#include "ArsenicSaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class ARSENIC_API UArsenicSaveGameObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FAudioSave AudioData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FGraphicsSave GraphicsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerSave PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FCharacterAttributes PlayerAttributes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FControlsSave ControlsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPylonStateSave PylonStateData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	TMap<FName, FDoorSave> DoorData;
};

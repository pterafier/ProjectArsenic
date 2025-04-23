// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "AudioSettingsMenu.generated.h"

/**
 * Does not need to be derived from ArsenicMenuBase because these are placed inside of the SettingsMenu which will handle back actions
 */
UCLASS()
class ARSENIC_API UAudioSettingsMenu : public UActivatableWidgetBase
{
	GENERATED_BODY()
	
};

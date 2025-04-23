// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "VideoSettingsMenu.generated.h"

/**
 * Does not need to be derived from ArsenicMenuBase because these are placed inside of the SettingsMenu which will handle back actions
 */
UCLASS()
class ARSENIC_API UVideoSettingsMenu : public UActivatableWidgetBase
{
	GENERATED_BODY()
	
};

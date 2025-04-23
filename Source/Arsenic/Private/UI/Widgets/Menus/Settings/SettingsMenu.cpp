// Copyright Silvergrove Studio


#include "UI/Widgets/Menus/Settings/SettingsMenu.h"
#include "Groups/CommonButtonGroupBase.h"

void USettingsMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	NavButtonGroup = NewObject<UCommonButtonGroupBase>(this);

	Buttons.Add(BTN_Controls);
	Buttons.Add(BTN_Video);
	Buttons.Add(BTN_Audio);
	
	NavButtonGroup->AddWidgets(Buttons);
	NavButtonGroup->SetSelectionRequired(true);
}

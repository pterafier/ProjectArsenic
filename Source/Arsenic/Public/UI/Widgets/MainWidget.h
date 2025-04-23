// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "MainWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStacksCleared);

class UCommonActivatableWidgetContainerBase;

/**
 * 
 */
UCLASS()
class ARSENIC_API UMainWidget : public UActivatableWidgetBase
{
	GENERATED_BODY()

public:
	
/*
* Variables
*/
	// Stores the currently pushed, top-most widget of the most recently pushed stack. Needs to be cast to the target widget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stacks")
	TObjectPtr<UCommonActivatableWidget> PushedWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetContainerBase* HUD_Stack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetContainerBase* InteractableHUD_Stack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetContainerBase* Menu_Stack;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetContainerBase* Popup_Stack;

	UPROPERTY(BlueprintAssignable)
	FOnStacksCleared OnStacksCleared;
	
/*
 * Methods
 */
	// Main method used to add a widget to the screen. Activating the widget shows the mouse cursor and sets the input mode to Game and UI
	UFUNCTION(BlueprintCallable, Category = "Stacks")
	UCommonActivatableWidget* PushWidgetToTargetStack(UCommonActivatableWidgetContainerBase* TargetStack,
		TSubclassOf<UCommonActivatableWidget> WidgetClass,
		bool bActivateWidget,
		bool DisableInput,
		bool FlushInput = false
	);

	/*
	 * Mainly called by PushWidgetToTargetStack, this method is useful for setting the input mode to Game and UI and showing the mouse cursor.
	 * DisableInput prevents the player character from moving around while the menu is open 
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputModeMenuOpened(UWidget* WidgetToFocus, bool DisableInput, bool FlushInput = false);

	// Sets input mode to Game Only, disables mouse cursor and automatically enables character input
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputModeMenuClosed();

	UFUNCTION(BlueprintCallable, Category = "Stacks")
	void RemoveWidget(UCommonActivatableWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Stacks")
	void ClearStacks();

	void NativeConstruct() override;

private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	bool NativeOnHandleBackAction() override;
	
	bool StacksCleared = false;
	
};

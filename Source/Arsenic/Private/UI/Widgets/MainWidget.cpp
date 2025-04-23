// Copyright Silvergrove Studio


#include "UI/Widgets/MainWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UCommonActivatableWidget* UMainWidget::PushWidgetToTargetStack(
    UCommonActivatableWidgetContainerBase* TargetStack, TSubclassOf<UCommonActivatableWidget> WidgetClass,
    bool bActivateWidget, bool DisableInput, bool FlushInput)
{
    if (bActivateWidget)
    {
        PushedWidget = TargetStack->AddWidget(WidgetClass);
        PushedWidget->ActivateWidget();
        SetInputModeMenuOpened(PushedWidget, DisableInput, FlushInput);
    }
    else
    {
        PushedWidget = TargetStack->AddWidget(WidgetClass);
        PushedWidget->ActivateWidget();
    }
    return PushedWidget;
}

void UMainWidget::SetInputModeMenuOpened(UWidget* WidgetToFocus, bool DisableInput, bool FlushInput)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    FInputModeGameAndUI InputModeGameAndUI;
    
    if (FlushInput)
    {
        PlayerController->FlushPressedKeys();
    }
    
    InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeGameAndUI.SetHideCursorDuringCapture(false);
    InputModeGameAndUI.SetWidgetToFocus(WidgetToFocus->TakeWidget());
    PlayerController->SetInputMode(InputModeGameAndUI);
    PlayerController->SetShowMouseCursor(true);
    
    if (DisableInput)
    {
        PlayerCharacter->DisableInput(PlayerController);
    }
}

void UMainWidget::SetInputModeMenuClosed()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

    FInputModeGameOnly InputModeGameOnly;
    PlayerController->SetInputMode(InputModeGameOnly);
    PlayerController->SetShowMouseCursor(false);
    PlayerCharacter->EnableInput(PlayerController);
}

void UMainWidget::RemoveWidget(UCommonActivatableWidget* Widget)
{
    Widget->DeactivateWidget();
    float MenuTransitionDuration = Menu_Stack->GetTransitionDuration();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this]()
        {
            PushedWidget = Menu_Stack->GetActiveWidget();
        },
        MenuTransitionDuration,
        false
    );
}

void UMainWidget::ClearStacks()
{
    Menu_Stack->ClearWidgets();
    Popup_Stack->ClearWidgets();
    SetInputModeMenuClosed();
    PushedWidget = nullptr;
    OnStacksCleared.Broadcast();
}

void UMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PushedWidget == nullptr;
    bAutomaticallyRegisterInputOnConstruction = true;
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (PushedWidget == nullptr && StacksCleared == true)
    {
        ClearStacks();
        StacksCleared = false;
    }
    else if (PushedWidget != nullptr)
    {
        StacksCleared = true;
    }
}

bool UMainWidget::NativeOnHandleBackAction()
{
    return true;
}

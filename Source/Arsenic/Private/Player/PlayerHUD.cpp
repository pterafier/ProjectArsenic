// Copyright Silvergrove Studio


#include "Player/PlayerHUD.h"
#include "UI/Widgets/MainWidget.h"
#include "Blueprint/UserWidget.h"
#include "Game/Save/ArsenicSaveGameSubsystem.h"
#include "Player/ArsenicPlayerController.h"
#include "UI/Widgets/HUD/ArsenicPlayerHUD.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

TSubclassOf<UActivatableWidgetBase> APlayerHUD::GetActiveWidgetClass()
{
	if (ArsenicMainWidget->PushedWidget)
	{
		return ArsenicMainWidget->PushedWidget->GetClass();
	}
	else
	{
		return nullptr;
	}
}

bool APlayerHUD::isMenuStackEmpty()
{
	return ArsenicMainWidget->Menu_Stack->GetNumWidgets() == 0;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeHUD();

	check(SaveIndicatorWidgetClass)
	UArsenicSaveGameSubsystem* GameInstanceSubsystem = GetGameInstance()->GetSubsystem<UArsenicSaveGameSubsystem>();
	GameInstanceSubsystem->SetSaveIndicatorWidgetClass(SaveIndicatorWidgetClass);
}

void APlayerHUD::InitializeHUD()
{
	check(ArsenicMainWidgetClass);
	check(ArsenicPlayerHUDClass);
	
	ArsenicMainWidget = CreateWidget<UMainWidget>(GetWorld()->GetFirstPlayerController(), ArsenicMainWidgetClass);
	ArsenicMainWidget->AddToViewport();

	UCommonActivatableWidgetContainerBase* TargetStack = ArsenicMainWidget->HUD_Stack;
	TSubclassOf<UCommonActivatableWidget> WidgetClass = ArsenicPlayerHUDClass;
	UCommonActivatableWidget* NewPlayerHUDWidget = ArsenicMainWidget->PushWidgetToTargetStack(TargetStack, WidgetClass, false, false);
	ArsenicPlayerHUD = Cast<UArsenicPlayerHUD>(NewPlayerHUDWidget); // Casting so we have an easy reference to the always-active ArsenicPlayerHUD

	Cast<AArsenicPlayerController>(GetOwningPlayerController())->HUDInitialized();
}

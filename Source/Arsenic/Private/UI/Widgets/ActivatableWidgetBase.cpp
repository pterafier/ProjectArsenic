// Copyright Silvergrove Studio

#include "UI/Widgets/ActivatableWidgetBase.h"

UActivatableWidgetBase::UActivatableWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UActivatableWidgetBase::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
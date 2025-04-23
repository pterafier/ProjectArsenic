// Copyright Silvergrove Studio


#include "UI/Widgets/ArsenicMenuBase.h"

#include "CommonButtonBase.h"
#include "Player/ArsenicPlayerController.h"
#include "UI/Widgets/MainWidget.h"

bool UArsenicMenuBase::NativeOnHandleBackAction()
{
	if (!ArsenicMainWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ArsenicMenuBase: ArsenicMainWidget is not valid, could not handle back action"));
		return true; // returning true so inputs don't get hung up - we want to move on if this is the case - not actually tested though so this might break something IDK :O
	}
	else
	{
		ArsenicMainWidget->RemoveWidget(this);
		return true;
	}
}

void UArsenicMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	ArsenicPlayerController = Cast<AArsenicPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ArsenicPlayerController)
	{
		ArsenicMainWidget = ArsenicPlayerController->ArsenicMainWidget;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ArsenicMenuBase: Cast to ArsenicPlayerController failed!"));
	}

	PlayerControllerSet(); // Call the blueprint implementable event so UI can safely bind to PlayerController delegates
}

void UArsenicMenuBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	bAutomaticallyRegisterInputOnConstruction = true; 
}

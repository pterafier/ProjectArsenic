// Copyright Silvergrove Studio


#include "UI/Components/NavBars/BottomNavBarBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHUD.h"
#include "UI/Widgets/ArsenicMenuBase.h"
#include "UI/Widgets/MainWidget.h"

void UBottomNavBarBase::RemoveParentWidget()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
	UArsenicMenuBase* ParentWidget = GetTypedOuter<UArsenicMenuBase>();
	UMainWidget* MainWidget = PlayerHUD->ArsenicMainWidget;

	if (ParentWidget && MainWidget)
	{
		MainWidget->RemoveWidget(ParentWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBottomNavBarBase: ParentWidget or MainWidget is null, could not activate back action"));
	}
}

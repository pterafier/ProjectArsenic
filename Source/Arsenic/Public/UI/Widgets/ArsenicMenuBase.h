// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "ArsenicMenuBase.generated.h"

class AArsenicPlayerController;
class UCommonButtonGroupBase;
class UMainWidget;
/**
 * Created to allow menus to properly handle back actions with ArsenicMainWidget - do not inherit from this if your widget doesn't need to handle back actions
 */
UCLASS()
class ARSENIC_API UArsenicMenuBase : public UActivatableWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerController> ArsenicPlayerController;
	

protected:
	virtual bool NativeOnHandleBackAction() override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerControllerSet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMainWidget> ArsenicMainWidget;

private:
	
	
};

// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ArsenicMenuBase.h"
#include "SettingsMenu.generated.h"

class UCommonButtonBase;
class UCommonButtonGroupBase;

/**
 * 
 */
UCLASS()
class ARSENIC_API USettingsMenu : public UArsenicMenuBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	TObjectPtr<UCommonButtonGroupBase> NavButtonGroup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Controls;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Video;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Audio;

	UPROPERTY(BlueprintReadOnly, Category = "Navigation")
	TArray<TObjectPtr<UCommonButtonBase>> Buttons;

protected:
	virtual void NativePreConstruct() override;
	
};

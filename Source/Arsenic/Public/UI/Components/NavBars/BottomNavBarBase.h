// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "BottomNavBarBase.generated.h"

/**
 * 
 */
UCLASS()
class ARSENIC_API UBottomNavBarBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void RemoveParentWidget();
	
};

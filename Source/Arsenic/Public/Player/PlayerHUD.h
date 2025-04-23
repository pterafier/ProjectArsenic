// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UActivatableWidgetBase;
class UArsenicPlayerHUD;
class UMainWidget;

/**
 * Pushes starting widgets to the screen at begin play
 */
UCLASS()
class ARSENIC_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UMainWidget> ArsenicMainWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UArsenicPlayerHUD> ArsenicPlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainWidget> ArsenicMainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UArsenicPlayerHUD> ArsenicPlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> SaveIndicatorWidgetClass;
/*
 * Methods
 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> GetActiveWidgetClass();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	bool isMenuStackEmpty();

protected:
	virtual void BeginPlay() override;

	void InitializeHUD();
	
};

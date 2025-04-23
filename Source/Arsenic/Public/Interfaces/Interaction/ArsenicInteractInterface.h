// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArsenicInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UArsenicInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARSENIC_API IArsenicInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void LookAt();

	virtual void Interact_Implementation() = 0;
	virtual void LookAt_Implementation() = 0;
};

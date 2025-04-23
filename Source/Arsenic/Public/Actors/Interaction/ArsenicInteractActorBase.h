// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interaction/ArsenicInteractInterface.h"
#include "ArsenicInteractActorBase.generated.h"

class AArsenicPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopLookingAt, AActor*, Actor);

UCLASS()
class ARSENIC_API AArsenicInteractActorBase : public AActor, public IArsenicInteractInterface
{
	GENERATED_BODY()
	
public:	
	AArsenicInteractActorBase();

/*
 * Public Variables
 */
	// Scene Root
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Used to include more than 1 mesh in the interaction highlight. Must be set in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	TArray<TObjectPtr<UStaticMeshComponent>> HighlightMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool UseHighlight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool AllowInteraction;

	// Text that will be displayed when the actor is being looked at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FText ActionText;

	// Controls whether the player can interact with the object or not
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool isInteractable;

	// Controls whether the player can interact with the object or not
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool isHighlighted;
	
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<AArsenicPlayerCharacter> ArsenicPlayerCharacter;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnStopLookingAt OnStopLookingAt;
	
/*
* Public Methods
*/
	// Highlights the static mesh(es)
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HighlightMesh();
	
	// Unhighlights the static mesh
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UnHighlightMesh();

	// Called when the actor gets highlighted. UseHighlight must be set to true
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BeginHighlight();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void EndHighlight();

	// Called when the actor is no longer being looked at by the player
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void StopLookingAt();
	virtual void StopLookingAt_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetAllowInteraction(bool bAllowInteraction, bool bUseHighlight);
	
protected:
	virtual void BeginPlay() override;

	void TryHighlightMesh();
	void TryUnhighlightMesh();

public:	
	virtual void Tick(float DeltaTime) override;

	/** Begin Interact Interface */
	virtual void Interact_Implementation() override;
	virtual void LookAt_Implementation() override;
	/** End Interact Interface */

private:
	// Timer handle to allow us to set a timer regarding highlighting/unhighlighting the mesh
	FTimerHandle UnHighlightTimerHandle;

};

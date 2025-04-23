// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Characters/ArsenicPlayerCharacter.h"
#include "Game/Systems/PylonManager.h"
#include "GameFramework/Actor.h"
#include "Interaction/Doors/ArsenicDoorBase.h"
#include "Player/ArsenicPlayerState.h"
#include "ArsenicCheckpoint.generated.h"

class UBoxComponent;

UCLASS()
class ARSENIC_API AArsenicCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AArsenicCheckpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerSave PlayerSaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FCharacterAttributes PlayerAttributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPylonStateSave PylonSaveData;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTriggerBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void FetchSaveData();

};

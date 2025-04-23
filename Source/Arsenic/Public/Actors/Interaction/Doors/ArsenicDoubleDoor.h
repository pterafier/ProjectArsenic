// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interaction/Doors/ArsenicDoorBase.h"
#include "ArsenicDoubleDoor.generated.h"

/**
 * 
 */
UCLASS()
class ARSENIC_API AArsenicDoubleDoor : public AArsenicDoorBase
{
	GENERATED_BODY()

public:
	AArsenicDoubleDoor();
	
	// Used only for double horizontal sliding doors. If you're using this, make sure to add it to the HighlightMeshes array!
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoubleDoorSide2;

	// Used for the other side of double sliding doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (MakeEditWidget = true, EditCondition = "DoorType == EDoorType::DoubleSliding", EditConditionHides))
	FVector OpenPoint2;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void CacheStartingPositions() override;
	virtual void CloseDoubleSlidingDoor() override;
	virtual void OpenDoubleSlidingDoor() override;

private:
	void InterpToDoubleSlidingDoorOpenPosition();
	void InterpToDoubleSlidingDoorClosedPosition();
	
	FVector StartingLocation_SecondDoor;
	
};

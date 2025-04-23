// Copyright Silvergrove Studio


#include "Actors/Interaction/Doors/ArsenicDoubleDoor.h"

#include "Components/ArrowComponent.h"

AArsenicDoubleDoor::AArsenicDoubleDoor()
{
	DoubleDoorSide2 = CreateDefaultSubobject<UStaticMeshComponent>("Double Door Side 2");
	DoubleDoorSide2->SetupAttachment(SceneComponent);

	OpenPoint2 = FVector(0, 0, 0);
	StartingLocation_SecondDoor = FVector(0, 0, 0);
}

void AArsenicDoubleDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DoorType = EDoorType::DoubleSliding;
	
	DistanceCheckInterval = SlidingDoorDistanceCheckInterval;
	//StaticMesh->SetRelativeLocation(FVector(0, -110, 0));
	//DoubleDoorSide2->SetRelativeLocation(FVector(0, 0, 0));
	//OpenPoint1 = FVector(0, 0, 0);
	//OpenPoint2 = FVector(0, 0, 0);
	Arrow->SetRelativeLocation(FVector(0, 0, 170));
	CloseDoorAutomatically = true;
}

void AArsenicDoubleDoor::CacheStartingPositions()
{
	Super::CacheStartingPositions();

	StartingLocation_SecondDoor = DoubleDoorSide2->GetRelativeLocation();
	UpdateDoorMap();
}

void AArsenicDoubleDoor::InterpToDoubleSlidingDoorOpenPosition()
{
	if (OpenAnimationStarted)
	{
		StaticMesh->SetRelativeLocation(FMath::VInterpTo(
			StaticMesh->GetRelativeLocation(),
			OpenPoint1,
			GetWorld()->GetDeltaSeconds(),
			DoorSpeed
		));
		DoubleDoorSide2->SetRelativeLocation(FMath::VInterpTo(
			DoubleDoorSide2->GetRelativeLocation(),
			OpenPoint2,
			GetWorld()->GetDeltaSeconds(),
			DoorSpeed
		));

		if (FVector::Dist(StaticMesh->GetRelativeLocation(), OpenPoint1) < 1.0f)
		{
			OpenAnimationStarted = false;
			GetWorldTimerManager().ClearTimer(DoorAnimationTimerHandle);

			OnDoorFinishOpen(); // Call the blueprint implementable event
			
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AArsenicDoorBase: Open Door Animation finished");
		}
	}
}

void AArsenicDoubleDoor::InterpToDoubleSlidingDoorClosedPosition()
{
	if (ClosedAnimationStarted)
	{
		FVector SecondDoorOffset = FVector(0, -0.5, 0);
		StaticMesh->SetRelativeLocation(FMath::VInterpTo(StaticMesh->GetRelativeLocation(), StartingLocation, GetWorld()->GetDeltaSeconds(), DoorSpeed));
		DoubleDoorSide2->SetRelativeLocation(FMath::VInterpTo(DoubleDoorSide2->GetRelativeLocation(), StartingLocation_SecondDoor + SecondDoorOffset, GetWorld()->GetDeltaSeconds(), DoorSpeed));

		if (FVector::Dist(StaticMesh->GetRelativeLocation(), StartingLocation) < 0.5f)
		{
			ClosedAnimationStarted = false;
			GetWorldTimerManager().ClearTimer(DoorAnimationTimerHandle);			
			OnDoorFinishClose(); // Call the blueprint implementable event

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AArsenicDoorBase: Close Door Animation finished");
		}
	}
}

void AArsenicDoubleDoor::CloseDoubleSlidingDoor()
{
	if (!ClosedAnimationStarted)
	{
		OpenAnimationStarted = false;
		ClosedAnimationStarted = true;

		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToDoubleSlidingDoorClosedPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);
	}
}

void AArsenicDoubleDoor::OpenDoubleSlidingDoor()
{
	if (!OpenAnimationStarted)
	{
		ClosedAnimationStarted = false;
		OpenAnimationStarted = true;
		
		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToDoubleSlidingDoorOpenPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);
	}
}

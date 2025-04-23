// Copyright Silvergrove Studio


#include "Actors/Interaction/Doors/ArsenicDoorBase.h"

#include "Characters/ArsenicPlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Game/Save/ArsenicSaveGameSubsystem.h"

AArsenicDoorBase::AArsenicDoorBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	SetRootComponent(SceneComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	StaticMesh->SetupAttachment(SceneComponent);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(GetRootComponent());
	
	DistanceToClose = 800.f;
	DistanceToOpen = 750.f;
	DistanceToPlayer = 0.f;
	DistanceCheckInterval = 0.5f;
	SlidingDoorDistanceCheckInterval = 0.5f;
	HingedDoorDistanceCheckInterval = 1.0f;
	DefaultDoorSpeed = 1.f;
	DoorSpeed = DefaultDoorSpeed;
	SlamSpeedMultiplier = 2.5;

	StartingForwardVector = FVector(0, 0, 0);
	StartingLocation = FVector(0, 0, 0);
	StartingRotation = FRotator(0, 0, 0);
	DoorRotationDirection = FRotator(0, 0, 0);
	OpenPoint1 = FVector(0, 0, 0);
	
	PrimaryActorTick.bCanEverTick = false;
	CloseDoorAutomatically = false;
	OpenAnimationStarted = false;
	DoorOpen = false;
	DoorLocked = false;
	StartOpened = false;
	DoorSlammed = false;
	RequiresStatCheck = false;
	StatInteractionSuccessful = false;
	BoundToOnGameLoadedDelegate = false;
	
	ActionText = FText::FromString("Open");
	DoorSaveData = FDoorSave();
}

void AArsenicDoorBase::OnSaveGameObjectLoaded_Callback()
{
	RegisterWithSaveSubsystem();
}

void AArsenicDoorBase::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (DoorType != EDoorType::Hinged) return;
	if (DoorLocked) return;

	if (DoorOpen)
	{
		TryCloseDoor();
	}
	else
	{
		TryOpenDoor();
	}
}

void AArsenicDoorBase::LookAt_Implementation()
{
	if (DoorType != EDoorType::Hinged) return;
	
	if (DoorOpen)
	{
		ActionText = FText::FromString("Close");
		UseHighlight = true;
	}
	else if (DoorLocked && StatInteractionSuccessful)
	{
		ActionText = FText::FromString("Locked");
		UseHighlight = false;
	}
	else
	{
		ActionText = FText::FromString("Open");
		UseHighlight = true;
	}
	
	Super::LookAt_Implementation(); // Calling super later to control UseHighlight
	OnActionTextChanged(); // Call the blueprint implementable event
}

void AArsenicDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
	StartDistanceCheckTimer();
	RegisterWithSaveSubsystem();
}

void AArsenicDoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	switch (DoorType)
	{
		case EDoorType::SingleSliding:
			DistanceCheckInterval = SlidingDoorDistanceCheckInterval;
			StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
			//OpenPoint1 = FVector(0, 0, 0);
			Arrow->SetRelativeLocation(FVector(0, 0, 170));
			CloseDoorAutomatically = true;
			break;
							
		case EDoorType::DoubleSliding:
			// Implemented in child class
			break;
							
		case EDoorType::Hinged:
			DistanceCheckInterval = HingedDoorDistanceCheckInterval;
			//StaticMesh->SetRelativeLocation(FVector(0, -110, 0)); // settable when final art is added
			Arrow->SetRelativeLocation(FVector(0, -110, 170));
			if (RequiresStatCheck)
			{
				StartOpened = false;
			}
			break;
							
		default:
			DistanceCheckInterval = SlidingDoorDistanceCheckInterval;
			break;
	}
}

FRotator AArsenicDoorBase::CalculateHingedDoorOpenDirection()
{
	FVector ToPlayer = (ArsenicPlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(StartingForwardVector, ToPlayer);

	DoorRotationDirection = (DotProduct > 0) ? FRotator(0, -90, 0) : FRotator(0, 90, 0);
	return DoorRotationDirection;
}

void AArsenicDoorBase::InterpToHingedDoorOpenPosition()
{
	if (OpenAnimationStarted)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DoorRotationDirection, GetWorld()->GetDeltaSeconds(), DoorSpeed));
	
		if (FMath::Abs(GetActorRotation().Yaw - DoorRotationDirection.Yaw) < 0.25)
		{
			OpenAnimationStarted = false;
			GetWorldTimerManager().ClearTimer(DoorAnimationTimerHandle);
			
			OnDoorFinishOpen(); // Call the blueprint implementable event
			
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AArsenicDoorBase: Open Door Animation finished");
		}
	}
}

void AArsenicDoorBase::InterpToSlidingDoorOpenPosition()
{
	if (OpenAnimationStarted)
	{
		StaticMesh->SetRelativeLocation(FMath::VInterpTo(
			StaticMesh->GetRelativeLocation(),
			OpenPoint1,
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

void AArsenicDoorBase::CheckPlayerDistance()
{
	if (!ArsenicPlayerCharacter) return;

	DistanceToPlayer = GetDistanceTo(ArsenicPlayerCharacter);

	if (CloseDoorAutomatically && DoorOpen && DistanceToPlayer >= DistanceToClose)
	{
		TryCloseDoor();
	}
	if (DoorType != EDoorType::Hinged && !DoorOpen && DistanceToPlayer <= DistanceToOpen)
	{
		TryOpenDoor();
	}
}

void AArsenicDoorBase::CloseSlidingDoor()
{
	if (!ClosedAnimationStarted)
	{
		OpenAnimationStarted = false;
		ClosedAnimationStarted = true;

		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToSlidingDoorClosedPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);
	}
}

void AArsenicDoorBase::CloseHingedDoor()
{
	if (!ClosedAnimationStarted)
	{
		OpenAnimationStarted = false;
		ClosedAnimationStarted = true;
		ActionText = FText::FromString("Open");
		OnActionTextChanged(); // Call the blueprint implementable event

		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToHingedDoorClosedPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);
	}
}

void AArsenicDoorBase::OpenSlidingDoor()
{
	if (!OpenAnimationStarted)
	{
		ClosedAnimationStarted = false;
		OpenAnimationStarted = true;
		
		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToSlidingDoorOpenPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);
	}
}

void AArsenicDoorBase::InterpToHingedDoorClosedPosition()
{
	if (ClosedAnimationStarted)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), StartingRotation, GetWorld()->GetDeltaSeconds(), DoorSpeed));

		if (FMath::Abs(GetActorRotation().Yaw - StartingRotation.Yaw) < 0.25)
		{
			ClosedAnimationStarted = false;
			GetWorldTimerManager().ClearTimer(DoorAnimationTimerHandle);
			
			OnDoorFinishClose(); // Call the blueprint implementable event

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AArsenicDoorBase: Close Door Animation finished");
		}
	}
}

void AArsenicDoorBase::InterpToSlidingDoorClosedPosition()
{
	if (ClosedAnimationStarted)
	{
		StaticMesh->SetRelativeLocation(FMath::VInterpTo(StaticMesh->GetRelativeLocation(), StartingLocation, GetWorld()->GetDeltaSeconds(), DoorSpeed));

		if (FVector::Dist(StaticMesh->GetRelativeLocation(), StartingLocation) < 1.0f)
		{
			ClosedAnimationStarted = false;
			GetWorldTimerManager().ClearTimer(DoorAnimationTimerHandle);
			
			OnDoorFinishClose(); // Call the blueprint implementable event

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AArsenicDoorBase: Close Door Animation finished");
		}
	}
}

void AArsenicDoorBase::SetStateOpened_Hinged()
{
	DoorOpen = true;
	StatInteractionSuccessful = true;
	SetActorRotation(StartingRotation - FRotator(0, 90, 0));
	UpdateDoorMap();
}

void AArsenicDoorBase::SetStateClosed_Hinged()
{
	DoorOpen = false;
	SetActorRotation(StartingRotation);
	UpdateDoorMap();
}

void AArsenicDoorBase::InitializeFirstTimeDoorState()
{
	CacheStartingPositions();
	
	if (StartOpened && !CloseDoorAutomatically)
	{
		SetStateOpened_Hinged();
	}

	if (!RequiresStatCheck)
	{
		StatInteractionSuccessful = true;
	}
	
	UpdateDoorMap();
	OnDoorStateLoaded();
}

void AArsenicDoorBase::StartDistanceCheckTimer()
{
	GetWorldTimerManager().SetTimer(
		DistanceToPlayerTimerHandle,
		this,
		&ThisClass::CheckPlayerDistance,
		DistanceCheckInterval,
		true
	);
}

void  AArsenicDoorBase::RegisterWithSaveSubsystem()
{
	UArsenicSaveGameSubsystem* SaveSubsystem = UArsenicSaveGameSubsystem::Get(this);

	if (SaveSubsystem->SaveObjectInitialized)
	{
		if (FDoorSave* FoundData = SaveSubsystem->CollectedDoorData.Find(UniqueDoorName))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is already registered, setting state from save data..."), *UniqueDoorName.ToString());
			SetDoorStateFromSaveData(*FoundData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is not registered, registering..."), *UniqueDoorName.ToString());
			FDoorSave EmptyDoorData;
			SaveSubsystem->CollectedDoorData.Add(UniqueDoorName, EmptyDoorData);
			InitializeFirstTimeDoorState();
		}
	}
	else
	{
		if (BoundToOnGameLoadedDelegate) return;
		SaveSubsystem->OnSaveGameObjectLoaded.AddDynamic(this, &ThisClass::OnSaveGameObjectLoaded_Callback);
		BoundToOnGameLoadedDelegate = true;
	}
}

void AArsenicDoorBase::CacheStartingPositions()
{
	StartingLocation = StaticMesh->GetRelativeLocation();
	StartingRotation = GetActorRotation();
	StartingForwardVector = GetActorForwardVector();
}

void AArsenicDoorBase::CloseDoubleSlidingDoor()
{
	// Implemented in child class
}

void AArsenicDoorBase::OpenDoubleSlidingDoor()
{
	// Implemented in child class
}

void AArsenicDoorBase::TryCloseDoor()
{
	if (DoorOpen)
	{
		DoorSpeed = DefaultDoorSpeed;
		
		switch (DoorType)
		{
			case EDoorType::SingleSliding:
				CloseSlidingDoor();
				DoorOpen = false;
				break;
						
			case EDoorType::DoubleSliding:
				CloseDoubleSlidingDoor();
				DoorOpen = false;
				break;
						
			case EDoorType::Hinged:
				CloseHingedDoor();
				DoorOpen = false;
				break;
						
			default:
				CloseSlidingDoor();
				DoorOpen = false;
				break;
		}
		OnDoorStartClose(); // Call the blueprint implementable event
		UpdateDoorMap();
	}
}

void AArsenicDoorBase::TryOpenDoor()
{
	if (!DoorOpen && !DoorLocked)
	{
		DoorSpeed = DefaultDoorSpeed;
		DoorSlammed = false;
		
		switch (DoorType)
		{
			case EDoorType::SingleSliding:
				OpenSlidingDoor();
				DoorOpen = true;
				break;
	
			case EDoorType::DoubleSliding:
				OpenDoubleSlidingDoor();
				DoorOpen = true;
				break;
							
			case EDoorType::Hinged:
				OpenHingedDoor();
				DoorOpen = true;
				break;
							
			default:
				OpenSlidingDoor();
				DoorOpen = true;
				break;
		}
		OnDoorStartOpen(); // Call the blueprint implementable event
		UpdateDoorMap();
	}
}

void AArsenicDoorBase::TrySlamDoor()
{
	if (DoorOpen)
	{
		DoorSpeed = DefaultDoorSpeed * SlamSpeedMultiplier;
		DoorSlammed = true;
		
		switch (DoorType)
		{
			case EDoorType::SingleSliding:
				CloseSlidingDoor();
				DoorOpen = false;
				break;

			case EDoorType::DoubleSliding:
				CloseDoubleSlidingDoor();
				DoorOpen = false;
				break;
							
			case EDoorType::Hinged:
				CloseHingedDoor();
				DoorOpen = false;
				break;
							
			default:
				CloseSlidingDoor();
				DoorOpen = false;
				break;
		}
		OnDoorStartSlam(); // Call the blueprint implementable event
	}
}

void AArsenicDoorBase::LockDoor()
{
	TrySlamDoor();
	DoorLocked = true;
	UseHighlight = false;
	ActionText = FText::FromString("Locked");
	OnDoorLocked(); // Call the blueprint implementable event
	OnActionTextChanged();
	UpdateDoorMap();
}

void AArsenicDoorBase::UnlockDoor()
{
	DoorLocked = false;
	UseHighlight = true;
	ActionText = FText::FromString("Open");
	OnDoorUnlocked(); // Call the blueprint implementable event
	OnActionTextChanged();
	UpdateDoorMap();
}

FDoorSave& AArsenicDoorBase::GetDoorSaveData()
{
	DoorSaveData.bStatInteractionSuccessful = StatInteractionSuccessful;
	DoorSaveData.bDoorLocked = DoorLocked;
	DoorSaveData.bIsOpened = DoorOpen;
	DoorSaveData.StartingLocation = StartingLocation;
	DoorSaveData.StartingRotation = StartingRotation;
	DoorSaveData.StartingForwardVector = StartingForwardVector;

	return DoorSaveData;
}

void AArsenicDoorBase::UpdateDoorMap()
{
	DoorSaveData = GetDoorSaveData();
	UArsenicSaveGameSubsystem* SaveSubsystem = UArsenicSaveGameSubsystem::Get(this);

	SaveSubsystem->UpdateCollectedDoorData(UniqueDoorName, DoorSaveData);
}

void AArsenicDoorBase::SetDoorStateFromSaveData(FDoorSave& FoundData)
{
	StatInteractionSuccessful = FoundData.bStatInteractionSuccessful;
	DoorLocked = FoundData.bDoorLocked;
	DoorOpen = FoundData.bIsOpened;
	StartingLocation = FoundData.StartingLocation;
	StartingRotation = FoundData.StartingRotation;
	StartingForwardVector = FoundData.StartingForwardVector;

	if (DoorOpen && DoorType == EDoorType::Hinged)
	{
		SetStateOpened_Hinged();
	}
	
	if (DoorLocked)
	{
		LockDoor();
	}

	OnDoorStateLoaded(); // call the blueprint implementable event
}

void AArsenicDoorBase::OpenHingedDoor()
{
	if (!OpenAnimationStarted)
	{
		if (!StartOpened)
		{
			CalculateHingedDoorOpenDirection();
		}
		else
		{
			DoorRotationDirection.Yaw = -90.f;
		}
		
		ClosedAnimationStarted = false;
		OpenAnimationStarted = true;
		ActionText = FText::FromString("Close");
		OnActionTextChanged(); // Call the blueprint implementable event
		
		GetWorldTimerManager().SetTimer(
			DoorAnimationTimerHandle,
			this,
			&ThisClass::InterpToHingedDoorOpenPosition,
			GetWorld()->GetDeltaSeconds(),
			true
		);

		StartOpened = false;
	}
}

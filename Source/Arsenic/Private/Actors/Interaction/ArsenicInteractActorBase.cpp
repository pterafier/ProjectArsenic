// Copyright Silvergrove Studio


#include "Actors/Interaction/ArsenicInteractActorBase.h"

#include "Characters/ArsenicPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AArsenicInteractActorBase::AArsenicInteractActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// INITIALIZE SCENE COMPONENT AND STATIC MESH IN CHILD CLASSES BY USING CreateDefaultSubobject<UCLASS>("COMPONENT_NAME")

	ActionText = FText::GetEmpty();
	isInteractable = false;
	AllowInteraction = true;
	UseHighlight = true;
	isHighlighted = false;
}

void AArsenicInteractActorBase::HighlightMesh()
{
	if (!HighlightMeshes.IsEmpty())
	{
		for (UStaticMeshComponent* Mesh : HighlightMeshes)
		{
			if (Mesh)
			{
				Mesh->SetRenderCustomDepth(true);
				if (Mesh->CustomDepthStencilValue != 250)
				{
					Mesh->SetCustomDepthStencilValue(250);
				}
			}
		}
	}
	else
	{
		if (StaticMesh)
		{
			StaticMesh->SetRenderCustomDepth(true);
			if (StaticMesh->CustomDepthStencilValue != 250)
			{
				StaticMesh->SetCustomDepthStencilValue(250);
			}
		}
	}
	isHighlighted = true;
	BeginHighlight(); // call the blueprint implementable event
}

void AArsenicInteractActorBase::UnHighlightMesh()
{
	if (!HighlightMeshes.IsEmpty())
	{
		for (UStaticMeshComponent* Mesh : HighlightMeshes)
		{
			Mesh->SetRenderCustomDepth(false);
		}
	}
	else
	{
		StaticMesh->SetRenderCustomDepth(false);
	}
	isHighlighted = false;
	EndHighlight(); // call the blueprint implementable event
}

void AArsenicInteractActorBase::StopLookingAt_Implementation()
{
	isInteractable = false;
}

void AArsenicInteractActorBase::SetAllowInteraction(bool bAllowInteraction, bool bUseHighlight)
{
	AllowInteraction = bAllowInteraction;
	UseHighlight = bUseHighlight;
}

void AArsenicInteractActorBase::BeginPlay()
{
	Super::BeginPlay();

	ArsenicPlayerCharacter = Cast<AArsenicPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AArsenicInteractActorBase::TryHighlightMesh()
{
	if (!UseHighlight) return;
	
	HighlightMesh();
}

void AArsenicInteractActorBase::TryUnhighlightMesh()
{
	if (ArsenicPlayerCharacter->LookAtActor == this) return;
	
	UnHighlightMesh();
	GetWorldTimerManager().ClearTimer(UnHighlightTimerHandle);
	OnStopLookingAt.Broadcast(this);
	StopLookingAt(); // Call the blueprint implementable event
}

void AArsenicInteractActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArsenicInteractActorBase::Interact_Implementation()
{
	// Implement in child classes or in blueprint
}

void AArsenicInteractActorBase::LookAt_Implementation()
{
	isInteractable = true;
	TryHighlightMesh();
	GetWorldTimerManager().SetTimer(UnHighlightTimerHandle, this, &ThisClass::TryUnhighlightMesh, GetWorld()->GetDeltaSeconds(), true);
}


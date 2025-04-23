// Copyright Silvergrove Studio


#include "Actors/ArsenicCheckpoint.h"

#include "Characters/ArsenicPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Game/Save/ArsenicSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ArsenicPlayerState.h"

AArsenicCheckpoint::AArsenicCheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(GetRootComponent());
}

void AArsenicCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBoxBeginOverlap);
}

void AArsenicCheckpoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnTriggerBoxBeginOverlap);
}

void AArsenicCheckpoint::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;
	
	UArsenicSaveGameSubsystem* SaveSubsystem = UArsenicSaveGameSubsystem::Get(this);
	if (!SaveSubsystem) return;
	if (!SaveSubsystem->CanSave) return; // helps prevent an unintentional save when the player transform is updated at begin play
	
	IArsenicSaveGameInterface* SaveGameInterface = SaveSubsystem;
	FetchSaveData();
	SaveGameInterface->Execute_SetPlayerSaveData(SaveSubsystem, PlayerSaveData);
	SaveGameInterface->Execute_SetPlayerAttributes(SaveSubsystem, PlayerAttributes);
	SaveGameInterface->Execute_SetPylonSaveData(SaveSubsystem, PylonSaveData);
	SaveGameInterface->Execute_SetDoorSaveData(SaveSubsystem, SaveSubsystem->CollectedDoorData);
	SaveGameInterface->Execute_SaveGameData(SaveSubsystem);
}


void AArsenicCheckpoint::FetchSaveData()
{
	AArsenicPlayerState* ArsenicPlayerState = Cast<AArsenicPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	AArsenicPlayerCharacter* ArsenicPlayerCharacter = Cast<AArsenicPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerSaveData = ArsenicPlayerState->GetPlayerSaveData();
	PlayerAttributes = ArsenicPlayerCharacter->GetAttributesSaveData();

	//Get PylonManager and fetch the current state
	if (UWorld* World = GetWorld())
	{
		if (const UPylonManager* PylonManager = UPylonManager::Get(World))
		{
			PylonSaveData = PylonManager->GetPylonSaveData();
		}
	}
}
// Copyright Silvergrove Studio


#include "Game/Systems/PylonManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Game/Save/ArsenicSaveGameSubsystem.h"
#include "Game/Save/ArsenicSaveGameObject.h"
#include "Kismet/GameplayStatics.h"

UPylonManager* UPylonManager::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UPylonManager>();
}

void UPylonManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!World) return;

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return;

	UArsenicSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UArsenicSaveGameSubsystem>();
	if (SaveSubsystem && SaveSubsystem->ArsenicSaveGameObject)
	{
		ActivePylons = SaveSubsystem->ArsenicSaveGameObject->PylonStateData.CurrentActivePylons;
	}
	else
	{
		ActivePylons = 3;
	}

	UE_LOG(LogTemp, Warning, TEXT("PylonManager Initialized. Active Pylons: %d"), ActivePylons);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("PylonManager Initialized"));
}

void UPylonManager::RemovePylon()
{
	if (ActivePylons > 0)
	{
		ActivePylons--;

		UE_LOG(LogTemp, Warning, TEXT("PylonManager Removed! Active Pylons Remaing: %d"), ActivePylons);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("PylonManager Removed! Active Pylons Remaining: %d"), ActivePylons));
		OnPylonStateChanged.Broadcast(ActivePylons);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("All Pylons are dowwn!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("All Pylons are already down!"));
	}
}

FPylonStateSave UPylonManager::GetPylonSaveData() const
{
	FPylonStateSave SaveData;
	SaveData.CurrentActivePylons = ActivePylons;
	return SaveData;
}



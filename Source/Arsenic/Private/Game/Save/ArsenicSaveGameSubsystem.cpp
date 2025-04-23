// Copyright Silvergrove Studio


#include "Game/Save/ArsenicSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Save/ArsenicSaveGameObject.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/Widgets/MainWidget.h"

UArsenicSaveGameSubsystem::UArsenicSaveGameSubsystem()
{
	ArsenicSaveGameObject = nullptr;
	SaveSlotName = TEXT("SaveSlot");
	SaveObjectClass = UArsenicSaveGameObject::StaticClass();

	DoesSaveExist = false;
	SaveObjectInitialized = false;
	CanSave = false;
}

UArsenicSaveGameSubsystem* UArsenicSaveGameSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UArsenicSaveGameSubsystem>();
}

void UArsenicSaveGameSubsystem::SetSaveIndicatorWidgetClass(TSubclassOf<UActivatableWidgetBase> WidgetClass)
{
	SaveIndicatorWidgetClass = WidgetClass;
}

void UArsenicSaveGameSubsystem::UpdateCollectedDoorData(FName DoorName, FDoorSave& DoorData)
{
	CollectedDoorData.Add(DoorName, DoorData);
}

void UArsenicSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadGameData_Implementation();
	//SetGraphicsSettingsFromSaveData();
}

void UArsenicSaveGameSubsystem::SetGraphicsSettingsFromSaveData()
{
	FGraphicsSave GraphicsSettings = ArsenicSaveGameObject->GraphicsData;
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	
	if (ArsenicSaveGameObject)
	{
		SetResolution();
		GameUserSettings->SetOverallScalabilityLevel(GraphicsSettings.QualityIndex);
		GameUserSettings->SetViewDistanceQuality(GraphicsSettings.ViewDistanceIndex);
		GameUserSettings->SetAntiAliasingQuality(GraphicsSettings.AntiAliasingIndex);
		GameUserSettings->SetPostProcessingQuality(GraphicsSettings.PostProcessingIndex);
		GameUserSettings->SetShadowQuality(GraphicsSettings.ShadowsIndex);
		GameUserSettings->SetTextureQuality(GraphicsSettings.TexturesIndex);
		GameUserSettings->SetVisualEffectQuality(GraphicsSettings.EffectsIndex);
		GameUserSettings->SetShadingQuality(GraphicsSettings.ShadingIndex);
		GameUserSettings->ApplySettings(false);
	}
}

void UArsenicSaveGameSubsystem::SetResolution()
{
	FGraphicsSave GraphicsSettings = ArsenicSaveGameObject->GraphicsData;
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	
	switch (GraphicsSettings.ResolutionIndex)
	{
	case 0:
		GameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case 1:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		break;
	case 2:
		GameUserSettings->SetScreenResolution(FIntPoint(2560, 1440));
		break;
	case 3:
		GameUserSettings->SetScreenResolution(FIntPoint(3840, 2160));
		break;
	default:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
	}
}

UArsenicSaveGameObject* UArsenicSaveGameSubsystem::GetSaveGameData_Implementation() const
{
	return ArsenicSaveGameObject;
}

void UArsenicSaveGameSubsystem::LoadGameData_Implementation()
{
	// Check if the save game exists
	DoesSaveExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	// If it does, load it asynchronously and bind to the OnGameLoaded delegate
	if (DoesSaveExist)
	{
		FAsyncLoadGameFromSlotDelegate OnGameLoadedDelegate;
		OnGameLoadedDelegate.BindUObject(this, &ThisClass::OnLoadGameCompleted);
		UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, OnGameLoadedDelegate);
	}
	// Otherwise, create a new save game object
	else
	{
		USaveGame* NewSaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveObjectClass);
		ArsenicSaveGameObject = Cast<UArsenicSaveGameObject>(NewSaveGameObject);

		SaveObjectInitialized = true;
		OnSaveGameObjectLoaded.Broadcast();
		CanSave = true;
	}
}

void UArsenicSaveGameSubsystem::SaveGameData_Implementation()
{
	PushSaveIndicatorWidget();
	
	FAsyncSaveGameToSlotDelegate OnGameSavedDelegate;
	OnGameSavedDelegate.BindUObject(this, &ThisClass::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(ArsenicSaveGameObject, SaveSlotName, 0, OnGameSavedDelegate);
	
}

void UArsenicSaveGameSubsystem::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (SaveIndicatorWidget)
	{
		AArsenicPlayerController* ArsenicPlayerController = Cast<AArsenicPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UMainWidget* ArsenicMainWidget = ArsenicPlayerController->ArsenicMainWidget;
		
		ArsenicMainWidget->RemoveWidget(SaveIndicatorWidget);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("UArsenicSaveGameSubsystem: SaveIndicatorWidget Popped"));
	}
}

void UArsenicSaveGameSubsystem::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex,
	USaveGame* LoadedSaveGameObject)
{
	ArsenicSaveGameObject = Cast<UArsenicSaveGameObject>(LoadedSaveGameObject);

	if (!ArsenicSaveGameObject)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10,
			FColor::Red,
			TEXT("UArsenicSaveGameSubsystem: Loading save game data failed, creating new save game object...")
		);
		USaveGame* NewSaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveObjectClass);
		ArsenicSaveGameObject = Cast<UArsenicSaveGameObject>(NewSaveGameObject);
	}

	// Doors use this map to keep track of their state, so we need to make sure it's populated by the save object before doors access it
	CollectedDoorData = ArsenicSaveGameObject->DoorData;
	
	SaveObjectInitialized = true;
	OnSaveGameObjectLoaded.Broadcast();
	CanSave = true;
}

void UArsenicSaveGameSubsystem::PushSaveIndicatorWidget()
{
	check(SaveIndicatorWidgetClass);
	
	AArsenicPlayerController* ArsenicPlayerController = Cast<AArsenicPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UMainWidget* ArsenicMainWidget = ArsenicPlayerController->ArsenicMainWidget;
	UCommonActivatableWidgetContainerBase* PopupStack = ArsenicMainWidget->Popup_Stack;

	SaveIndicatorWidget = ArsenicMainWidget->PushWidgetToTargetStack(PopupStack, SaveIndicatorWidgetClass, false, false);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("UArsenicSaveGameSubsystem: SaveIndicatorWidget Pushed"));
}

void UArsenicSaveGameSubsystem::SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData)
{
	ArsenicSaveGameObject->PlayerData = PlayerData;
}

void UArsenicSaveGameSubsystem::SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes)
{
	ArsenicSaveGameObject->PlayerAttributes = PlayerAttributes;
}

void UArsenicSaveGameSubsystem::SetControlsSaveData_Implementation(const FControlsSave& ControlsData)
{
	ArsenicSaveGameObject->ControlsData = ControlsData;
}

void UArsenicSaveGameSubsystem::SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData)
{
	ArsenicSaveGameObject->GraphicsData = GraphicsData;
}

void UArsenicSaveGameSubsystem::SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData)
{
	ArsenicSaveGameObject->AudioData = AudioData;
}

void UArsenicSaveGameSubsystem::SetPylonSaveData_Implementation(const FPylonStateSave& PylonData)
{
	ArsenicSaveGameObject->PylonStateData = PylonData;
}

void UArsenicSaveGameSubsystem::SetDoorSaveData_Implementation(const TMap<FName, FDoorSave>& DoorData)
{
	ArsenicSaveGameObject->DoorData = DoorData;
}

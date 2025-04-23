// Copyright Silvergrove Studio


#include "Player/ArsenicPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/ArsenicPlayerCharacter.h"
#include "GameFramework/HUD.h"
#include "Interfaces/Interaction/ArsenicInteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHUD.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "UI/Widgets/MainWidget.h"

AArsenicPlayerController::AArsenicPlayerController()
{
	BaseTurnRate = 1.0f;
	InvertY = false;
	isMenuActivated = false;
}

void AArsenicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(GameInputMappingContext, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("Mapping context added successfully"));
	}

	SetReferences();
	SetupInputComponent(InputComponent);
}

void AArsenicPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void AArsenicPlayerController::SetupInputComponent(UInputComponent* InInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, UGameplayStatics::GetPlayerCharacter(this, 0), &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, UGameplayStatics::GetPlayerCharacter(this, 0), &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		// Opening and closing pause menu
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::HandlePauseMenu);

		EnhancedInputComponent->BindAction(TabLeftAction, ETriggerEvent::Started, this, &AArsenicPlayerController::OnTabLeftPressed);
		EnhancedInputComponent->BindAction(TabRightAction, ETriggerEvent::Started, this, &ThisClass::OnTabRightPressed);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::CallPlayerInteract);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("SetupInputComponent() failed!"));
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AArsenicPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	PlayerPawn->AddMovementInput(PlayerPawn->GetActorForwardVector(), MovementVector.Y);
	PlayerPawn->AddMovementInput(PlayerPawn->GetActorRightVector(), MovementVector.X);
}

void AArsenicPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	PlayerPawn->AddControllerYawInput(LookAxisVector.X * BaseTurnRate);
	
	float InvertedPitchInput = LookAxisVector.Y * BaseTurnRate * -1;
	float PitchInput = LookAxisVector.Y * BaseTurnRate;

	// if InvertY = true, set Result to InvertedPitchInput
	float Result = InvertY ? InvertedPitchInput : PitchInput; 
	PlayerPawn->AddControllerPitchInput(Result);
}

void AArsenicPlayerController::HandlePauseMenu()
{
	TSubclassOf<UActivatableWidgetBase> ActiveWidget = PlayerHUDActor->GetActiveWidgetClass();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("PlayerController: Pause Menu button triggered"));
	
	if (ActiveWidget == PauseMenu || PlayerHUDActor->isMenuStackEmpty())
	{
		isMenuActivated = !isMenuActivated;
		if (isMenuActivated)
		{
			SwitchToMenuInput();
			UCommonActivatableWidgetContainerBase* TargetStack = ArsenicMainWidget->Menu_Stack;
			PauseMenuInstance = ArsenicMainWidget->PushWidgetToTargetStack(TargetStack, PauseMenu, true, true);
		}
		else
		{
			if (ActiveWidget == PauseMenu)
			{
				ArsenicMainWidget->RemoveWidget(PauseMenuInstance.Get());
				PauseMenuInstance = nullptr;
			}
		}
	}
}

void AArsenicPlayerController::OnStacksCleared()
{
	isMenuActivated = false;
	SwitchToGameplayInput();
}

void AArsenicPlayerController::SetReferences()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	ArsenicPlayerCharacter = Cast<AArsenicPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	check(PauseMenu);
}

void AArsenicPlayerController::CallPlayerInteract() // rider is lying, do not make this const else it won't bind the input
{
	ArsenicPlayerCharacter->Interact();
}

void AArsenicPlayerController::HUDInitialized()
{
	PlayerHUDActor = Cast<APlayerHUD>(GetHUD());
	ArsenicMainWidget = PlayerHUDActor->ArsenicMainWidget;

	check(ArsenicMainWidget);
	ArsenicMainWidget->OnStacksCleared.AddDynamic(this, &ThisClass::OnStacksCleared);
}

void AArsenicPlayerController::SwitchToGameplayInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->RemoveMappingContext(MenuInputMappingContext); // Remove only menu controls
		InputSubsystem->AddMappingContext(GameInputMappingContext, 0); // Ensure gameplay input remains
	}
}

void AArsenicPlayerController::SwitchToMenuInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->RemoveMappingContext(GameInputMappingContext); // Remove gameplay controls
		InputSubsystem->AddMappingContext(MenuInputMappingContext, 1); // Higher priority for UI to block gameplay inputs - redundancy
	}
}

void AArsenicPlayerController::OnTabLeftPressed()
{
	OnLeftTabPressed.Broadcast();
}

void AArsenicPlayerController::OnTabRightPressed()
{
	OnRightTabPressed.Broadcast();
}

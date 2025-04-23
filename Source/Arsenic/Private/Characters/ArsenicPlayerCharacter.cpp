// Copyright Silvergrove Studio


#include "Characters/ArsenicPlayerCharacter.h"

#include "Interfaces/Interaction/ArsenicInteractInterface.h"
#include "AbilitySystem/ArsenicAbilitySystemComponent.h"
#include "AbilitySystem/ArsenicAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Player/ArsenicPlayerController.h"
#include "Player/ArsenicPlayerState.h"

AArsenicPlayerCharacter::AArsenicPlayerCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetMesh());

	InteractDistance = 500.f;
}

void AArsenicPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetPlayerReferences(NewController);
	InitAbilityActorInfo();
}

FCharacterAttributes AArsenicPlayerCharacter::GetAttributesSaveData()
{
	FCharacterAttributes PlayerAttributes;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	PlayerAttributes.MaxHealth = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxHealthAttribute());
	PlayerAttributes.Health = ASC->GetNumericAttribute(UArsenicAttributeSet::GetHealthAttribute());
	PlayerAttributes.MaxMana = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxManaAttribute());
	PlayerAttributes.Mana = ASC->GetNumericAttribute(UArsenicAttributeSet::GetManaAttribute());
	PlayerAttributes.Esoteric = ASC->GetNumericAttribute(UArsenicAttributeSet::GetEsotericAttribute());
	PlayerAttributes.MaxEsoteric = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxEsotericAttribute());
	PlayerAttributes.Strength = ASC->GetNumericAttribute(UArsenicAttributeSet::GetStrengthAttribute());
	PlayerAttributes.MaxStrength = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxStrengthAttribute());
	PlayerAttributes.Perception = ASC->GetNumericAttribute(UArsenicAttributeSet::GetPerceptionAttribute());
	PlayerAttributes.MaxPerception = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxPerceptionAttribute());
	PlayerAttributes.Resolve = ASC->GetNumericAttribute(UArsenicAttributeSet::GetResolveAttribute());
	PlayerAttributes.MaxResolve = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxResolveAttribute());
	PlayerAttributes.HealthRegen = ASC->GetNumericAttribute(UArsenicAttributeSet::GetHealthRegenAttribute());
	PlayerAttributes.ManaRegen = ASC->GetNumericAttribute(UArsenicAttributeSet::GetManaRegenAttribute());
	PlayerAttributes.PhysicalResistance = ASC->GetNumericAttribute(UArsenicAttributeSet::GetPhysicalResistanceAttribute());
	PlayerAttributes.MagicResistance = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMagicResistanceAttribute());
	PlayerAttributes.WalkSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetWalkSpeedAttribute());
	PlayerAttributes.MaxWalkSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxWalkSpeedAttribute());
	PlayerAttributes.SprintSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetSprintSpeedAttribute());
	PlayerAttributes.MaxSprintSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetMaxSprintSpeedAttribute());
	PlayerAttributes.LookSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetLookSpeedAttribute());
	PlayerAttributes.BaseMaxHealth = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseMaxHealthAttribute());
	PlayerAttributes.BaseMaxMana = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseMaxManaAttribute());
	PlayerAttributes.BaseEsoteric = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseEsotericAttribute());
	PlayerAttributes.BaseStrength = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseStrengthAttribute());
	PlayerAttributes.BasePerception = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBasePerceptionAttribute());
	PlayerAttributes.BaseResolve = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseResolveAttribute());
	PlayerAttributes.BaseWalkSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseWalkSpeedAttribute());
	PlayerAttributes.BaseSprintSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseSprintSpeedAttribute());
	PlayerAttributes.BaseLookSpeed = ASC->GetNumericAttribute(UArsenicAttributeSet::GetBaseLookSpeedAttribute());
	
	return PlayerAttributes;
}

void AArsenicPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractTrace();
}

void AArsenicPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddStartupAbilities();
}

void AArsenicPlayerCharacter::AddStartupAbilities()
{
	UArsenicAbilitySystemComponent* ArsenicASC = CastChecked<UArsenicAbilitySystemComponent>(ArsenicAbilitySystemComponent);
	ArsenicASC->AddStartupAbilities(StartupAbilities);
}

void AArsenicPlayerCharacter::InitAbilityActorInfo()
{
	check(ArsenicPlayerState);
	ArsenicAbilitySystemComponent = ArsenicPlayerState->GetAbilitySystemComponent();
	ArsenicAttributeSet = ArsenicPlayerState->GetAttributeSet();
	ArsenicAbilitySystemComponent->InitAbilityActorInfo(ArsenicPlayerState, this);
	
	Cast<UArsenicAbilitySystemComponent>(ArsenicPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet(); // Inform the ASC that the character is initialized
}

void AArsenicPlayerCharacter::InteractTrace()
{
	AActor* HitActor = PerformInteractLineTrace();

	if (HitActor == LookAtActor) return;
	
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UArsenicInteractInterface::StaticClass()))
	{
		LookAtActor = HitActor;
		OnPlayerLookAt.Broadcast(HitActor);
		IArsenicInteractInterface::Execute_LookAt(HitActor);
	}
	else
	{
		LookAtActor = nullptr;
	}
}

AActor* AArsenicPlayerCharacter::PerformInteractLineTrace()
{
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * InteractDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

	return (bHit && HitResult.IsValidBlockingHit() && HitResult.GetActor()) ? HitResult.GetActor() : nullptr;
}

void AArsenicPlayerCharacter::Interact()
{
	if (LookAtActor == nullptr) return;

	if (LookAtActor->GetClass()->ImplementsInterface(UArsenicInteractInterface::StaticClass()))
	{
		IArsenicInteractInterface::Execute_Interact(LookAtActor);
	}
}

void AArsenicPlayerCharacter::SetPlayerReferences(AController* NewController)
{
	ArsenicPlayerController = Cast<AArsenicPlayerController>(NewController);
	ArsenicPlayerState = Cast<AArsenicPlayerState>(GetPlayerState<AArsenicPlayerState>());
	
	check(ArsenicPlayerController);
	check(ArsenicPlayerState);
}

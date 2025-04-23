// Copyright Silvergrove Studio


#include "Characters/ArsenicCharacterBase.h"

AArsenicCharacterBase::AArsenicCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AArsenicCharacterBase::GetAbilitySystemComponent() const
{
	return ArsenicAbilitySystemComponent;
}

void AArsenicCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArsenicCharacterBase::InitAbilityActorInfo()
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SHeroComponent.h"
#include "SPawnExtensionComponent.h"

USHeroComponent::USHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bPawnHasInitialized = false;
}


void USHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (USPawnExtensionComponent* PawnExtComp = USPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExtComp->OnPawnReadyToInitialize_RegisterAndCall(
				FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnReadyToInitialize));
		}
	}
	else
	{
		UE_LOG(
			LogTemp, 
			Error, 
			TEXT("[USHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}
}

bool USHeroComponent::IsPawnComponentReadyToInitialize() const
{
	const APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return false;
	}

	return true;
}

void USHeroComponent::OnPawnReadyToInitialize()
{
	if (!ensure(!bPawnHasInitialized))
	{
		// Do not initialize twice
		return;
	}

	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	bPawnHasInitialized = true;
}

void USHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool USHeroComponent::HasPawnInitialized() const
{
	return bPawnHasInitialized;
}
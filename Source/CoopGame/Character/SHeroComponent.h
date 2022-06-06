// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPawnComponent.h"
#include "InputActionValue.h"
#include "SHeroComponent.generated.h"

class UInputComponent;
struct FInputActionValue;

/**
 * A component used to create a player controlled pawns (characters, vehicles, etc..).
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class COOPGAME_API USHeroComponent : public USPawnComponent
{
	GENERATED_BODY()

public:

	USHeroComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the hero component if one exists on the specific actor.
	UFUNCTION(BlueprintPure, Category="Hero")
	static USHeroComponent* FindHeroComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<USHeroComponent>() : nullptr);
	}

	/** True if this has completed OnPawnReadyToInitialize so is prepared for late added features */
	bool HasPawnInitialized() const;

protected:

	virtual void OnRegister() override;

	virtual bool IsPawnComponentReadyToInitialize() const override;
	void OnPawnReadyToInitialize();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	// True when the pawn has fully finished initialization
	bool bPawnHasInitialized;
};

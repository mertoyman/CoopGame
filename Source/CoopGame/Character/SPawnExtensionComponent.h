// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPawnComponent.h"
#include "SPawnExtensionComponent.generated.h"

class USPawnData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSDynamicMulticastDelegate);

/**
 * Component used to add functionality to all Pawn classes.
 */
UCLASS()
class COOPGAME_API USPawnExtensionComponent : public USPawnComponent
{
	GENERATED_BODY()

public:

	USPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the pawn extension component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category="Pawn")
	static USPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<USPawnExtensionComponent>() : nullptr);
	}

	template <class T>
	const T* GetPawnData() const
	{
		return Cast<T>(PawnData);
	}

	void SetPawnData(const USPawnData* InPawnData);

	// Call this anytime the pawn needs to check if it's ready to be initialized (pawn data assigned, possessed, etc..). 
	bool CheckPawnReadyToInitialize();

	// Returns true if the pawn is ready to be initialized.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsPawnReadyToInitialize() const { return bPawnReadyToInitialize; }

	// Register with the OnPawnReadyToInitialize delegate and broadcast if condition is already met.
	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

protected:

	virtual void OnRegister() override;

	// Delegate fired when pawn has everything needed for initialization.
	FSimpleMulticastDelegate OnPawnReadyToInitialize;

	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Pawn Ready To Initialize"))
	FSDynamicMulticastDelegate BP_OnPawnReadyToInitialize;

	// Pawn data used to create the pawn. Specified from a spawn function or on a placed instance.
	UPROPERTY(EditInstanceOnly, Category = "Shooter|Pawn")
	const USPawnData* PawnData;

	int32 bPawnReadyToInitialize : 1;
};

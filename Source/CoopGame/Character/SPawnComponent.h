// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SPawnComponent.generated.h"

UINTERFACE(BlueprintType)
class COOPGAME_API USReadyInterface : public UInterface
{
	GENERATED_BODY()
};

class ISReadyInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPawnComponentReadyToInitialize() const = 0;
};
/**
 * SPawnComponent
 *
 * An actor component that can be used for adding custom behavior to pawns.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class COOPGAME_API USPawnComponent : public UPawnComponent, public ISReadyInterface
{
	GENERATED_BODY()

public:
	USPawnComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool IsPawnComponentReadyToInitialize() const override { return true; }
};

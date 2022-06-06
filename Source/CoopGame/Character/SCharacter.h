// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "SCharacter.generated.h"

class ASPlayerController;
class USPawnExtensionComponent;

/**
 *  The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 */
UCLASS(Config = Game, meta=(ShortTooltip = "The base character pawn class used by this project"))
class COOPGAME_API ASCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:

	ASCharacter(const FObjectInitializer& ObjectInitializer);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooter | Character", meta = (AllowPrivateAccess = true))
	USPawnExtensionComponent* PawnExtComponent;
};

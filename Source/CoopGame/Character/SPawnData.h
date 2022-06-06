// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SPawnData.generated.h"

class APawn;


/**
 * Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, meta=(DisplayName="Shooter Pawn Data", ShortTooltip="Data asset used to define a Pawn"))
class COOPGAME_API USPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	USPawnData(const FObjectInitializer& ObjectInitializer);

	// Class to instantiate for this pawn (should usually derive from ASPawn or ASCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pawn")
	TSubclassOf<APawn> PawnClass;
};

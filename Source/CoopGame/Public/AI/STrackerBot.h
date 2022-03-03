// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UStaticMeshComponent* MeshComp;

	FVector GetNextPathPoint();

	// Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditAnywhere, Category="AI")
	float ForceAmount;

	// Distance from target to stop movement
	UPROPERTY(EditAnywhere, Category="AI")
	float RequiredDistanceToTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};

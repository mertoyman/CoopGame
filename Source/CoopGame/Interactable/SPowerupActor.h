// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnActivated();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnPowerUpTicked();

	UFUNCTION()
	void OnTickPowerUp();
	

private:

	/* Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, Category=PowerUps)
	float PowerupInterval;

	/* Total time in seconds we apply the powerup effect */
	UPROPERTY(EditDefaultsOnly, Category=PowerUps)
	int32 TotalNumberOfTicks;

	FTimerHandle TimerHandle_PowerUpTick;

	/* Total number of ticks applied */
	int32 TicksProcessed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PowerUps", meta=(AllowPrivateAccess=true))
	bool bActivated;

public:
	void ActivatePowerUp();
};

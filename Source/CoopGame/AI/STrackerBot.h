// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class USphereComponent;
class USoundCue;

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

	UPROPERTY(EditDefaultsOnly, Category="AI")
	USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	USphereComponent* SphereComp;

	FVector GetNextPathPoint();
	
	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* OwningHealthComponent, float Health, float DeltaHealth,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	void SelfDestruct();

	void DamageSelf();

	void OnCheckNearbyBots();

private:
	// Dynamic material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UParticleSystem* ExplosionEffect;

	// Distance from target to stop movement
	UPROPERTY(EditAnywhere, Category="AI")
	float RequiredDistanceToTarget;
	
	// Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditAnywhere, Category="AI")
	float ForceAmount;

	UPROPERTY(EditAnywhere, Category="AI")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, Category="AI")
	float DamageRadius;

	UPROPERTY(EditAnywhere, Category="AI")
	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	USoundCue* ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	USoundCue* SelfDestructSound;

	FTimerHandle TimerHandle_SelfDamage;

	UPROPERTY(EditAnywhere, Category="AI")
	float ExplosionTimerInSeconds;

	bool bStartedSelfDestruction;

	UPROPERTY(VisibleAnywhere, Category="AI")
	float PowerLevel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

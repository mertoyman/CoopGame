// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SGrenadeLauncher.h"
#include "Grenade.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class AGrenade : public AActor
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);
	
private:	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* MovementComp;

public:
	AGrenade();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<class ASExplosionEffect> ExplosionTemplate;

	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> MyController;
	
	//projectile data
	struct FProjectileWeaponData ProjectileConfig;
	
	void Explode(const FHitResult& Impact);
	void DisableAndDestroy();

public:	
	void LaunchProjectile() const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class ASCharacter;
class UParticleSystem;
class USkeletalMeshComponent;

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 AmmoPerClip;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int32 InitialClips;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int32 MaxAmmo;

	FWeaponData()
	{
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
	}
	
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ASCharacter* MyPawn;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UCameraShakeBase> FireCamShake;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	int32 RemainingBullets;
	
	UPROPERTY(EditDefaultsOnly, Category="Config")
	FWeaponData WeaponConfig;

	uint8 bWantsToFire : 1;

	EWeaponState::Type CurrentState;

	FVector MuzzleLocation;

	
	bool WeaponTrace(FHitResult& Hit, FVector TraceStart, FVector TraceEnd) const;

	virtual void PlayFireEffects();

	virtual void OnBurstStarted();

	virtual void OnBurstFinished();

	void DetermineWeaponState();

	void SetWeaponState(EWeaponState::Type NewState);

	void HandleFiring();

	virtual void Fire() PURE_VIRTUAL(ASWeapon::Fire, );
	
	void UseAmmo();

public:
	void SetOwningPawn(ASCharacter* WeaponOwner);

	void StartFire();
	
	void StopFire();
	
	void Reload();
};

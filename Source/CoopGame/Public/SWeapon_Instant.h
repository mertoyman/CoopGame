// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeapon_Instant.generated.h"

UENUM()
namespace EFireMode
{
	enum Mode
	{
		Semi_Auto,
		Auto,
		Burst
	};	
}

USTRUCT()
struct FInstantWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="WeaponStat")
	TEnumAsByte<EFireMode::Mode> DefaultFireMode;
	
	UPROPERTY(EditDefaultsOnly, Category="WeaponStat")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category="WeaponStat")
	float WeaponRange;

	/* RPM - Bullets per minute fired */
	UPROPERTY(EditDefaultsOnly, Category="WeaponStat")
	float RateOfFire;

	UPROPERTY(EditAnywhere, Category="WeaponStat",meta=(EditCondition="DefaultFireMode==EFireMode::Burst", Tooltip="Amount of bullets shot in single fire", ClampMin="2", ClampMax="4"))
	int32 BurstFireAmount;

	UPROPERTY(EditAnywhere, Category="WeaponStat", meta=(EditCondition="DefaultFireMode!=EFireMode::Burst", ToolTip ="If true, this rifle has both full-auto and semi-auto fire modes"))
	bool IsSelectiveFireRifle;

	FInstantWeaponData()
	{
		DefaultFireMode = EFireMode::Auto;
		BaseDamage = 20;
		WeaponRange = 10000.f;
		RateOfFire = 600;
		IsSelectiveFireRifle = false;
		BurstFireAmount = 4;
	}
};

UCLASS(Abstract, Blueprintable)
class COOPGAME_API ASWeapon_Instant : public ASWeapon
{
	GENERATED_BODY()

public:
	ASWeapon_Instant();

protected:
	
	void DealDamage(FHitResult Impact, FVector ShotDirection);

	void BurstFire();

	virtual void OnBurstFinished() override;

	void StartFiring();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	/* Derived from RateOfFire */
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category="Config")
	FInstantWeaponData InstantConfig;

	int8 CurrentBurstFire;

	uint8 IsSingleFire : 1;

	uint8 IsBurstFire : 1;

	/* Bullet Spread in Degrees */
	UPROPERTY(EditDefaultsOnly, Category="Weapon", meta=(ClampMin=0.f))
	int BulletSpread;

	virtual void Fire() override;

public:
	void SwitchFireMode();
};

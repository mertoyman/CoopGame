// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SWeapon.h"
#include "GameFramework/DamageType.h"
#include "SGrenadeLauncher.generated.h"

USTRUCT()
struct FProjectileWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<class AGrenade> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	int32 ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ExplosionRadius;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<UDamageType> DamageType;

	FProjectileWeaponData()
	{
		ProjectileClass = NULL;
		ExplosionDamage = 200;
		ExplosionRadius = 100.f;
		DamageType = UDamageType::StaticClass();
	}
	
};

UCLASS(Abstract)
class COOPGAME_API ASGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
	
public:
	void ApplyProjectileData(FProjectileWeaponData& Data) const;

protected:
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category="Config")
	FProjectileWeaponData ProjectileConfig;

};

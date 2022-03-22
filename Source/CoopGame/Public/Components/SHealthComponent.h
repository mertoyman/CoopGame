// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	// OnHealthChanged event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComponent*, HealthComp, float, Health, float, DeltaHealth,
		const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category="HealthComponent")
	void Heal(float HealingAmount);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealthComponent",meta=(AllowPrivateAccess=true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponent",meta=(AllowPrivateAccess=true))
	float MaxHealth;
	
public:	
	UPROPERTY(BlueprintAssignable, Category= "Events")
	FOnHealthChangedSignature OnHealthChanged;

	FORCEINLINE float GetHealth() const { return Health; }
};

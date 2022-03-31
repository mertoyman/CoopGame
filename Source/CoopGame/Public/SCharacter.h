// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Components/SHealthComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class ASWeapon;
class USHealthComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UUserWidget* CrosshairWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);

	void BeginRun();

	void EndRun();
	
	void BeginCrouch();

	void EndCrouch();

	void BeginJump();

	void SetEquipped(bool bNewEquipped);

	void SetTargeting(bool bNewTargeting);
	
	void OnStartTargeting();

	void OnStopTargeting();

	UFUNCTION(BlueprintCallable)
	void StartWeaponFire();

	UFUNCTION(BlueprintCallable)
	void StopWeaponFire();

	void SwitchFireMode();

	void ReloadMagazine();
	
	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category="Game/Weapon")
	bool IsEquippedWeaponSingleHanded() const;

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(USHealthComponent* USHealthComponent, float Health, float DeltaHealth,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	USHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, Category="CameraSettings")
	float ViewPitchMin;

	UPROPERTY(EditAnywhere, Category="CameraSettings")
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomFOV = 65.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 bIsTargeting : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 bIsEquipped : 1;
	
	UPROPERTY(EditDefaultsOnly, Category="Player", meta=(ClampMin=0.1f, ClampMax = 100.0f));
	float ZoomInterpSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<class UUserWidget> CrossHairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess=true))
	bool bDied;
	
	UCharacterMovementComponent* CharacterMovementComp;

	ASWeapon* CurrentWeapon;

	FName SocketName;

	uint8 bWantsToFire : 1;
	
	uint8 bWantsToZoom : 1;
	
	float DefaultFOV;
	
public:
	
};


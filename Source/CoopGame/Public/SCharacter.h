// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class ASWeapon;

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

	void StartWeaponFire();
	
	void StopWeaponFire();

	void SwitchFireMode();

	void ReloadMagazine();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category="CameraSettings")
	float ViewPitchMin;

	UPROPERTY(EditAnywhere, Category="CameraSettings")
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomFOV = 65.0f;

	UPROPERTY()
	uint8 bIsTargeting : 1;

	UPROPERTY(EditDefaultsOnly, Category="Player", meta=(ClampMin=0.1f, ClampMax = 100.0f));
	float ZoomInterpSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<class UUserWidget> CrossHairWidgetClass;

	UCharacterMovementComponent* CharacterMovementComp;

	ASWeapon* CurrentWeapon;

	FName SocketName;
	
	virtual FVector GetPawnViewLocation() const override;

	uint8 bWantsToFire : 1;
	
	uint8 bWantsToZoom : 1;

	uint8 bIsEquipped : 1;
	
	float DefaultFOV;

	UFUNCTION(BlueprintCallable, Category="Game/Weapon")
	bool IsTargeting() const;

	UFUNCTION(BlueprintCallable, Category="Game/Weapon")
	bool IsEquipped() const;

	UFUNCTION(BlueprintCallable, Category="Game/Weapon")
	bool IsEquippedWeaponSingleHanded() const;
};


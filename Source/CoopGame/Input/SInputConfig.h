// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "SInputConfig.generated.h"


/**
 * FSInputAction
 *
 * Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FSInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * USInputConfig
 *
 * Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class COOPGAME_API USInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	USInputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bond.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputAction"))
	TArray<FSInputAction> NativeInputActions;

	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputAction"))
	TArray<FSInputAction> AbilityInputActions;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPawnComponent.h"

USPawnComponent::USPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"


#include "SWeapon_Instant.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));	
	CameraComp->SetupAttachment(SpringArmComp);

    ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
    ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	bWantsToFire = false;
	bIsTargeting = false;
	
	
	SocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController)
	{
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = ViewPitchMin;
			PlayerController->PlayerCameraManager->ViewPitchMax = ViewPitchMax;
		}
	}

	DefaultFOV = CameraComp->FieldOfView;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, SpawnParams);

	if(CurrentWeapon)
	{
		SetEquipped(true);
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);	
	}
	else
	{
		SetEquipped(false);
	}
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

bool ASCharacter::IsTargeting() const
{
	return bIsTargeting;
}

bool ASCharacter::IsEquipped() const
{
	return bIsEquipped;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = bIsTargeting ? ZoomFOV : DefaultFOV;
	const float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}


void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(CameraComp->GetForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(CameraComp->GetRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginJump()
{
	Jump();
}

void ASCharacter::SetEquipped(bool bNewEquipped)
{
	bIsEquipped = bNewEquipped;
}

void ASCharacter::SetTargeting(bool bNewTargeting)
{
	bIsTargeting = bNewTargeting;
}

void ASCharacter::OnStartTargeting()
{
	SetTargeting(true);
}

void ASCharacter::OnStopTargeting()
{
	SetTargeting(false);
}

void ASCharacter::StartWeaponFire()
{
	if(!bWantsToFire)
	{
		bWantsToFire = true;
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();	
		}
	}
}

void ASCharacter::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}
	}
}

void ASCharacter::SwitchFireMode()
{
	if (CurrentWeapon)
	{
		auto Ss = Cast<ASWeapon_Instant>(CurrentWeapon);
		if(Ss)
			Ss->SwitchFireMode();
	}
}

void ASCharacter::ReloadMagazine()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::BeginJump);
	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ASCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ASCharacter::OnStopTargeting);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartWeaponFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopWeaponFire);
	PlayerInputComponent->BindAction("SwitchFireMode", IE_Pressed, this, &ASCharacter::SwitchFireMode);	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this,  &ASCharacter::ReloadMagazine);
}

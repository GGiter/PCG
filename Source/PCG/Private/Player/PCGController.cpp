// Fill out your copyright notice in the Description page of Project Settings.

#include "PCGController.h"
#include "PCGPlayer.h"
#include "Interactable.h"

void APCGController::BeginPlay()
{
	Super::BeginPlay();
}

void APCGController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveX", this, &APCGController::MoveX);
		InputComponent->BindAxis("MoveY", this, &APCGController::MoveY);
		InputComponent->BindAxis("LookPitch", this, &APCGController::AddPitchInput);
		InputComponent->BindAxis("LookYaw", this, &APCGController::AddYawInput);
		InputComponent->BindAction("Use", IE_Pressed, this, &APCGController::Use);
	}
}
void APCGController::MoveX(float Value)
{
	if (PlayerRef == NULL)
	{
		PlayerRef = Cast<APCGPlayer>(GetPawn());
	}
	else if (PlayerRef)
	{
		if ((PlayerRef->GetIsMovementEnabled()) && (Value != 0.0f))
		{
			const FRotator Rotation = ControlRotation;
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			Value = FMath::Clamp<float>(Value, -1.0f, 1.0f);
			PlayerRef->AddMovementInput(Direction, Value);
		}
	}
}
void APCGController::MoveY(float Value)
{
	if (PlayerRef == NULL)
	{
		PlayerRef = Cast<APCGPlayer>(GetPawn());
	}
	else if (PlayerRef)
	{
		if ((PlayerRef->GetIsMovementEnabled()) && (Value != 0.0f))
		{
			const FRotator Rotation = ControlRotation;
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			Value = FMath::Clamp<float>(Value, -1.0f, 1.0f);
			PlayerRef->AddMovementInput(Direction, Value);
		}
	}
}
void APCGController::Use()
{
	if (PlayerRef == NULL)
	{
		PlayerRef = Cast<APCGPlayer>(GetPawn());
		
	}
	else if (PlayerRef)
	{

		AInteractable *Interactable = PlayerRef->FindFocusedActor();
		if (Interactable)
		{
			Interactable->OnInteract(PlayerRef);
		}
	}
}
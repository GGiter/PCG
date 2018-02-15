// Fill out your copyright notice in the Description page of Project Settings.
#include "PCGPlayer.h"
#include "PCG.h"
#include "PCGController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interactable.h"

// Sets default values
APCGPlayer::APCGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);

	bIsMovementEnabled = true;
	InteractionDistance = 400.f;
}

// Called when the game starts or when spawned
void APCGPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		PlayerHUD->AddToViewport();
	}
}

// Called every frame
void APCGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Controller && Controller->IsLocalController())
	{
		AInteractable* Usable = FindFocusedActor();
		// End Focus
		if (FocusedActor != Usable)
		{
			if (FocusedActor)
			{
				FocusedActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}
		// Assign new Focus
		FocusedActor = Usable;
		// Start Focus.
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
	}


}

// Called to bind functionality to input
void APCGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APCGPlayer::HandleHighlight()
{
	AInteractable* NewHighlight = FindFocusedActor();
	if (NewHighlight)
	{
		if (NewHighlight != FocusedActor) {
			if (FocusedActor) FocusedActor->OnEndFocus();
			NewHighlight->OnBeginFocus();
			FocusedActor = NewHighlight;
		}
	}
	else {
		if (FocusedActor) FocusedActor->OnEndFocus();
		FocusedActor = nullptr;
	}
}
AInteractable* APCGPlayer::FindFocusedActor()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractionDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<AInteractable>(Hit.GetActor());
}


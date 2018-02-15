// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PCGPlayer.generated.h"

UCLASS()
class PCG_API APCGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCGPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE 
		UPROPERTY(BluerpintPure,Category=Movement)
		bool GetIsMovementEnabled() const { return bIsMovementEnabled; }

	class AInteractable* FindFocusedActor();

	FORCEINLINE
		UPROPERTY(BlueprintPure,Category=HUD)
		class UUserWidget* GetPlayerHUD() const { return PlayerHUD; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		 class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		 class UCameraComponent* PlayerCamera;
	UPROPERTY(BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* PlayerHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> PlayerHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
		float InteractionDistance;
	bool bHasNewFocus;
	bool bIsMovementEnabled;
	class AInteractable* FocusedActor;
	void HandleHighlight();
	
};

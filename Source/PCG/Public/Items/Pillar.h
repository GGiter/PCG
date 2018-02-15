// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pillar.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API APillar : public AInteractable
{
	GENERATED_BODY()
public: 
	APillar();
	virtual void BeginPlay() override;
	virtual void OnInteract_Implementation(AActor*Caller) override;
protected:
	UPROPERTY(EditDefaultsOnly)
	 TSubclassOf<class AFractalActor> FractalClass;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Scene;

	
	
};

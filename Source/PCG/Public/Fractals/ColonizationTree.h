// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fractals/FractalActor.h"
#include "Fractals/SCTree.h"
#include "ColonizationTree.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API AColonizationTree : public AFractalActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Test();
	void SpawnLines();
private:
	TArray<TPair<FVector, FVector> > Locations;
	bool bC = false;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FractalActor.h"
#include "MengerSponge.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API AMengerSponge : public AFractalActor
{
	GENERATED_BODY()
    public:
		AMengerSponge();
		virtual void Generate() override;
		void GenerateBoxes();
		virtual void BeginPlay() override;
	private:
		float r;
		FVector Position;
		int32 Iterations;
		
	
	
};

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
		UFUNCTION(BlueprintCallable)
		virtual void Generate() override;
		void GenerateBoxes();
		void GenerateTransforms(FTransform BoxTransform,TArray<FTransform>& TransformArray);
		void SpawnBoxes();
		virtual void BeginPlay() override;
	private:
		UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
			float r;
		UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
			float SizeR;
		FVector Position;
		int32 Iterations;
		TArray<UStaticMeshComponent*> Boxes;
		TArray<FTransform> Transforms;
		
	
	
};

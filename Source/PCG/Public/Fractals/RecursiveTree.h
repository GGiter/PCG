// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FractalActor.h"
#include "RecursiveTree.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API ARecursiveTree : public AFractalActor
{
	GENERATED_BODY()
	
public:

	ARecursiveTree();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Generation)
		virtual void Generate() override;
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Distance;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Intermission;
	void GenerateRecursive(float Start, float End, float Height,float Direction, int32 Lvl);
	
	
};

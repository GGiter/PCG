// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FractalActor.h"
#include "CantorSet.generated.h"

UCLASS()
class PCG_API ACantorSet : public AFractalActor
{
	GENERATED_BODY()
	
public:	
	
	ACantorSet();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = Generation)
		virtual void Generate() override;
protected:
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAcess="true"))
		UParticleSystem* PSC;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Distance;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Intermission;
	TArray<class UParticleSystemComponent*> Beams;
		void GenerateRecursive(float Start, float End, float Height);
	
	
};

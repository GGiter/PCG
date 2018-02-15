// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FractalActor.h"
#include "LSystemFractal.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPair
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly)
	FString First;
	UPROPERTY(EditDefaultsOnly)
	FString Second;

	FPair()
	{

	}

	FPair(FString NewFirst, FString NewSecond)
	{
		First = NewFirst;
		Second = NewSecond;
	}
};
UCLASS()
class PCG_API ALSystemFractal : public AFractalActor
{
	GENERATED_BODY()
public:
	void SpawnLine(FVector Location1, FVector Location2);
	UFUNCTION(BlueprintCallable)
	void LoadRules();
	FString GenerateSequence(int32 Length);
	virtual void Generate() override;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float InputAngleX;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float InputAngleY;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float InputAngleZ;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FPair> InputRules;
	TMap<TCHAR, FString>  Rules;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FString Axiom;
	
	
	
};

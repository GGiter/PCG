// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FractalUI.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API UFractalUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetFractalRef(TSubclassOf<class AFractalActor> NewFractalClass, FVector Location, FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	void SetIsFractalSet(bool NewIsFractalSet);
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Fractal, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AFractalActor> FractalClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fractal, meta = (AllowPrivateAccess = "true"))
		class AFractalActor* FractalRef;
	UFUNCTION(BlueprintCallable)
		void CallGenerate();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fractal, meta = (AllowPrivateAccess = "true"))
	bool IsFractalSet = false;

	void SetUp(FVector Location, FRotator Rotation);
	
};

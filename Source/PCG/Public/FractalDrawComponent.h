// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Engine/Public/PrimitiveSceneProxy.h"
#include "FractalDrawComponent.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API UFractalDrawComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	class AFractalActor* FractalActorRef;
	
	
};

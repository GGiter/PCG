// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCGController.generated.h"

/**
 * 
 */
UCLASS()
class PCG_API APCGController : public APlayerController
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
public:
	void SetupInputComponent() override;
private:
	void MoveX(float Value);
	void MoveY(float Value);
	void Use();
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class APCGPlayer* PlayerRef;
	
	
};

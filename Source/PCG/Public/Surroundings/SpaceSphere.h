// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "Public/Surroundings/SpaceSphereAlg.h"
#include "SpaceSphere.generated.h"

UCLASS()
class PCG_API ASpaceSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	void UpdateMaterial();
	UFUNCTION(BlueprintCallable)
		void Test();
private:
	TArray<UMaterialInstanceDynamic*> mDynamicMaterials;
	UTexture2D* mDynamicTexture;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"))
	UTexture2D* textureToReadFrom;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category = Galaxy)
		float StarsDensity=0.003f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category = Galaxy)
		float StarsBrightness = 0.18f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category=Galaxy)
		FVector2D NebulaScaleRange = { 0.001f,0.003f };
	uint32 mDataSize;
	uint32 mDataSqrtSize;

	FUpdateTextureRegion2D* mUpdateTextureRegion;
	uint8* mDynamicColors;
	uint8* mTextureColors;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
		UStaticMeshComponent* StaticMeshComponent;
	bool bC = false;
	int32 w, h;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "DSMapGenerator.generated.h"

UCLASS()
class PCG_API ADSMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSMapGenerator();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Generate();
	UFUNCTION(BlueprintCallable)
		void SetLandscapeScale();
	UFUNCTION(BlueprintCallable)
		void SetDetail(float NewDetail);
	UFUNCTION(BlueprintCallable)
		void SetRoughness(float NewRoughness);
	UFUNCTION(BlueprintPure)
		float GetDetail() const;
	UFUNCTION(BlueprintPure)
		float GetRoughness() const;
	UFUNCTION(BlueprintPure)
		FName GetLandscapeName() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LandscapeScale;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	void DrawMesh();
	void Divide(float Size);
	void Diamond(float x, float y, float Size, float Offset);
	void Square(float x, float y, float Size, float Offset);
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
	UParticleSystem* PSC;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	URuntimeMeshComponent* RuntimeMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Detail=3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Roughness=0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector Scale = { 1.0f,1.0f,1.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName LandscapeName="DiamondSquare Fractal";
	
	float WorldSize;
	float Max;
	TArray<FVector> PointList;


	TMap<TPair<float, float>, float> HeightMap;
	
	
};

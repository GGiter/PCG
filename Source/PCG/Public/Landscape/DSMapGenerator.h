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
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Detail=3.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Roughness=0.7f;
	
	float WorldSize;
	float Max;
	TArray<FVector> PointList;


	TMap<TPair<float, float>, float> HeightMap;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "PerlinMap.generated.h"

UCLASS()
class PCG_API APerlinMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinMap();
	void GenerateHeightMap();
	void RenderLandscape();
	UFUNCTION(BlueprintCallable)
	void Generate();
	UFUNCTION(BlueprintCallable)
		void SetRows(int32 NewRows);
	UFUNCTION(BlueprintCallable)
		void SetColumns(int32 NewColumns);
	UFUNCTION(BlueprintCallable)
		void SetBlockWidth(float NewBlockWidth);
	UFUNCTION(BlueprintPure)
		int32 GetRows() const;
	UFUNCTION(BlueprintPure)
		int32 GetColumns() const;
	UFUNCTION(BlueprintPure)
		float GetBlockWidth() const;
	UFUNCTION(BlueprintPure)
		FName GetLandscapeName() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Rows;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Columns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float BlockWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName LandscapeName;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URuntimeMeshComponent* RuntimeMesh;
	TArray<float> Terrain;
	TArray<FVector> PointList;
	
	
};

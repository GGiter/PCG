// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackAlg.h"
#include "Classes/Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "RuntimeMeshComponent.h"
#include "TrackGenerator.generated.h"

UCLASS()
class PCG_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackGenerator();
	UFUNCTION(BlueprintCallable)
		void Generate();
	UFUNCTION(BlueprintCallable)
		void SetSteepness(float NewSteepness);
	UFUNCTION(BlueprintCallable)
		void SetNewSpacing(float NewSpacing);
	UFUNCTION(BlueprintCallable)
		void SetNumberOfPoints(int32 NewNumberOfPoints);
	UFUNCTION(BlueprintCallable)
		void SetEllipseWidth(float NewEllipseWidth);
	UFUNCTION(BlueprintCallable)
		void SetEllpiseHeight(float NewEllipseHeight);
	UFUNCTION(BlueprintPure)
		float GetSteepness() const;
	UFUNCTION(BlueprintPure)
		float GetSpacing() const;
	UFUNCTION(BlueprintPure)
		int32 GetNumberOfPoints() const;
	UFUNCTION(BlueprintPure)
		float GetEllipseWidth() const;
	UFUNCTION(BlueprintPure)
		float GetEllipseHeight() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
		USplineComponent* PathSpline;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		UStaticMesh* RoadMesh;

	TArray<FVector> TrackPoints;
	TArray<TArray<TArray<FVector> > > SurroundingsHM;
	
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Steepness=0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float Spacing=200.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float TangentScale=1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		bool bAutoSpacing = false;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float StartingHeight = 3000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		int32 NumberOfPoints=6;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"),Category=Ellipse)
		float Ellipse_Width=30000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"),Category=Ellipse)
		float Ellipse_Height=30000.0f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"))
		TArray<USplineMeshComponent*> TrackMeshes;
	bool bC = true;
	void BuildSpline();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Public/Voronoi.h"
#include "Runtime/Core/Public/HAL/ThreadSafeCounter.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
/**
 * 
 */
class PCG_API FTrackAlg : public FRunnable
{
public:
	static FTrackAlg* Runnable;

	FRunnableThread* Thread;

	FThreadSafeCounter StopTaskCounter;
	bool IsFinished() const
	{
		return bFinished;
	}
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();

	static FTrackAlg* JoyInit(TArray<FVector>& NewTrack, TArray<TArray<TArray<FVector> >>& Surroundings, float NewEllipse_Width, float NewEllipse_Height, float NewNumberOfPoints, float NewStartingHeight, float NewSteepness);

	static void Shutdown();

	static bool IsThreadFinished();

	FTrackAlg(TArray<FVector>& NewTrack, TArray<TArray<TArray<FVector> >>& Surroundings,float NewEllipse_Width,float NewEllipse_Height,float NewNumberOfPoints,float NewStartingHeight,float NewSteepness);
	~FTrackAlg();
private:
	TArray<FEdge> VoronoiEdges;
	TArray<TArray<TArray<FVector> >>* SurroundingsHM;
	TArray<FVector> TrackTemp;
	FVector2D GetRandomPointInEllipse();
	void GenerateHeightMap();
	float GetAngleBetweenTwoVectors(FVector A, FVector B);
	void CreateTrack();
	TArray<FVector2D> GeneratePoints();
	bool bFinished = false;
	float Ellipse_Width, Ellipse_Height, NumberOfPoints, StartingHeight;
	float Steepness;
	TArray<FVector>* Track;
	TArray<FVector2D> Points;
};

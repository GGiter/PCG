// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackAlg.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"
#include "PerlinHeightMap.h"

FTrackAlg* FTrackAlg::Runnable = NULL;

FTrackAlg::FTrackAlg(TArray<FVector>& NewTrack,TArray<TArray<TArray<FVector> >>& Surroundings, float NewEllipse_Width, float NewEllipse_Height, float NewNumberOfPoints, float NewStartingHeight, float NewSteepness)
{
	Track = &NewTrack;
	SurroundingsHM = &Surroundings;
	Ellipse_Width = NewEllipse_Width;
	Ellipse_Height = NewEllipse_Height;
	NumberOfPoints = NewNumberOfPoints;
	StartingHeight = NewStartingHeight;
	Steepness = NewSteepness;
	Thread = FRunnableThread::Create(this, TEXT("FTrackAlg"), 0, TPri_AboveNormal);
}

FTrackAlg::~FTrackAlg()
{
	delete Thread;
	Thread = nullptr;
}

FVector2D FTrackAlg::GetRandomPointInEllipse()
{
	float t = 2.0f*PI*FMath::FRand();
	float u = FMath::FRand() + FMath::FRand();
	float r;
	if (u > 1.0f)
	{
		r = 2.0f - u;
	}
	else
	{
		r = u;
	}
	return FVector2D(Ellipse_Width*r*FMath::Cos(t) / 2.0f, Ellipse_Height*r*FMath::Sin(t) / 2.0f);
}

void FTrackAlg::GenerateHeightMap()
{
	for (int32 i = 0; i < TrackTemp.Num() - 1; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size of Voronoi1 %s"), *TrackTemp[i].ToString());
		UE_LOG(LogTemp, Warning, TEXT("Size of Voronoi2 %s"), *TrackTemp[i+1].ToString());
		PerlinHeightMap* Perlin1 = new PerlinHeightMap(TrackTemp[i]+FVector(0.0f, 1385 / 2.0f,0.0f), TrackTemp[i + 1] + FVector(0.0f, 1385 / 2.0f, 0.0f), 500.0f, 10, 10, 10.0f);
		PerlinHeightMap* Perlin2 = new PerlinHeightMap(TrackTemp[i] + FVector(0.0f, -1385 / 2.0f, 0.0f), TrackTemp[i + 1] + FVector(0.0f, -1385 / 2.0f, 0.0f), 500.0f, 10, 10, 10.0f);
		SurroundingsHM->Add(Perlin1->GetHeightMap());
		SurroundingsHM->Add(Perlin2->GetHeightMap());
	}
}

float FTrackAlg::GetAngleBetweenTwoVectors(FVector A, FVector B)
{
	A.Normalize();
	B.Normalize();
	return FMath::RadiansToDegrees(FGenericPlatformMath::Acos(FVector::DotProduct(A, B)));
	
}

void FTrackAlg::CreateTrack()
{
	Voronoi VoronoiAlg;
	VoronoiEdges = VoronoiAlg.GetEdges(GeneratePoints(), Ellipse_Width, Ellipse_Width);
	/*
	Temp.Sort([this](const FVector2D& A, const FVector2D& B)
	{
		return A.X < B.X;
	});
	*/

	TArray<FEdge> Temp;
	for (int32 i=0;i<VoronoiEdges.Num();++i)
	{

		
		if (TrackTemp.Num() == 0)
		{
			TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[0]), StartingHeight));
			TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[1]), StartingHeight));
			StartingHeight *= Steepness;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Angle %f"), GetAngleBetweenTwoVectors(FVector(FVector2D(TrackTemp.Top()), 0.0f), VoronoiEdges[i].Vertex[0]));
			if (GetAngleBetweenTwoVectors(FVector(FVector2D(TrackTemp.Top()),0.0f), VoronoiEdges[i].Vertex[1]) > 20.0f&&FVector::Dist(TrackTemp.Top(), VoronoiEdges[i].Vertex[0]) > 400.0f)
			{
				TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[0]),StartingHeight));
				TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[1]), StartingHeight));
				StartingHeight *= Steepness;
			}

				
		}
	
		
	}
/*
	for (int32 i = 0; i < Temp.Num(); ++i)
	{
		Track->Add(FVector(Temp[i], StartingHeight));

		
	}
*/
}

TArray<FVector2D> FTrackAlg::GeneratePoints()
{
	TArray<FVector2D> Generated;
	for (int32 i = 0; i < NumberOfPoints; ++i)
	{
		//Generated.Add(GetRandomPointInEllipse());
		Generated.Add(FVector2D(FMath::FRand()*Ellipse_Height, FMath::FRand()*Ellipse_Height));
	}
	return Generated;
}

bool FTrackAlg::Init()
{
	double Secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 Seed = (int32)(((int64)Secs) % INT_MAX);
	FMath::RandInit(Seed);
	return true;
}

uint32 FTrackAlg::Run()
{
	FPlatformProcess::Sleep(0.03);
	CreateTrack();
	GenerateHeightMap();
	for (FVector Vector : TrackTemp)
	{
		Track->Add(Vector);
	}

	bFinished = true;
	return 0;
}

void FTrackAlg::Stop()
{
	StopTaskCounter.Increment();
}

void FTrackAlg::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

FTrackAlg * FTrackAlg::JoyInit(TArray<FVector>& NewTrack, TArray<TArray<TArray<FVector> >>& Surroundings, float NewEllipse_Width, float NewEllipse_Height, float NewNumberOfPoints, float NewStartingHeight, float NewSteepness)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTrackAlg(NewTrack,Surroundings, NewEllipse_Width, NewEllipse_Height, NewNumberOfPoints, NewStartingHeight, NewSteepness);
	}
	return Runnable;
}

void FTrackAlg::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FTrackAlg::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Public/Landscape/Delaunay.h"
#include "Runtime/Core/Public/HAL/ThreadSafeCounter.h"

/**
 * 
 */

class PCG_API FDungeonAlg : public FRunnable
{
	static FDungeonAlg* Runnable;

	FRunnableThread* Thread;

	TArray<TArray<FVector> >* Locations;

	FThreadSafeCounter StopTaskCounter;
public:
	FDungeonAlg(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells,float NewRadius, TArray<TArray<int32>>& NewNumOfTypes, TArray<TArray<FEdge> >& NewRoomsBorders, TArray<TArray<FEdge> >& NewHubsBorders,float NewHallwayWidth);
	bool IsFinished() const
	{
		return bFinished;
	}
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();

	static FDungeonAlg* JoyInit(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells, float NewRadius, TArray<TArray<int32>>& NewNumOfTypes, TArray<TArray<FEdge> >& NewRoomsBorders, TArray<TArray<FEdge> >& NewHubsBorders, float NewHallwayWidth);

	static void Shutdown();

	static bool IsThreadFinished();

	FVector2D GetRandomPointInCircle();
	FVector2D GetRandomPointInEllipse();
	bool ValueInRange(float Value, float Min, float Max);
	float NormalRand(float Min,float Max);
	void GenerateCells();
	FVector2D SeperateCells(FBox2D& Cell);
	FVector2D CohaseCells(FBox2D& Cell);
	void ShortenEdge(FEdgeD& Edge, FBox2D Cell);
	void ShortenHallways(TArray<FBox2D>& Rooms, TArray<FBox2D>& Hallways);
	TArray<FBox2D> ThickenHallways(TArray<FEdgeD> HallwaysEdges,float Thickness);
	TArray<FBox2D> ShortenHallway(FBox2D& Hallway, FBox2D& Room, TArray<FEdge>& Borders);
	bool DoCellContains(FBox2D& Cell, FVector2D Point);
	void DetermineMainHubs();
	TArray<FEdgeD> GenerateGraph();
	TArray<FBox2D> CreateHallways(TArray<FEdgeD> TreeEdges,TArray<FBox2D>& Rooms);
	TArray<FBox2D> FindHallwaysRooms(TArray<FBox2D>& Hallways);
	TArray<FBox2D> BuildHallways(TArray<FBox2D> Rooms, FBox2D Hallway);
	~FDungeonAlg();
private:
	bool bFinished = false;
	uint32 NumberOfCells;
	float Radius;
	TArray<FBox2D> Cells;
	TArray<FBox2D> Hubs;
	TArray<TArray<int32>>* NumOfTypes;
	TArray<TArray<FEdge> >* RoomsBorders;
	TArray<TArray<FEdge> >* HubsBorders;
	float HallwayWidth=40.0f;
	float Ellipse_Width, Ellipse_Height;
};

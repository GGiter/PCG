// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Public/Landscape/Delaunay.h"
#include "Runtime/Core/Public/HAL/ThreadSafeCounter.h"

/**
 * 
 */
struct FCells
{
	FCells(FVector2D _P1, FVector2D _P2, FVector2D _P3, FVector2D _P4)
		: P1(_P1), P2(_P2), P3(_P3), P4(_P4)
	{
		Width = FVector2D::Distance(P1, P2);
		Height = FVector2D::Distance(P1, P4);
	}
	FVector2D P1, P2, P3, P4;
	float Width, Height;

};
inline bool operator == (const FCells &E1, const FCells &E2)
{	
	bool Centre = (E1.P1.X + E1.P2.X + E1.P3.X + E1.P4.X == E2.P1.X + E2.P2.X + E2.P3.X + E2.P4.X) && (E1.P1.Y + E1.P2.Y + E1.P3.Y + E1.P4.Y == E2.P1.Y + E2.P2.Y + E2.P3.Y + E2.P4.Y);
	return Centre&&(FVector2D::Distance(E1.P1,E1.P4)==FVector2D::Distance(E2.P1,E2.P4))&& (FVector2D::Distance(E1.P1, E1.P2) == FVector2D::Distance(E2.P1, E2.P2));
}
class PCG_API FDungeonAlg : public FRunnable
{
	static FDungeonAlg* Runnable;

	FRunnableThread* Thread;

	TArray<TArray<FVector> >* Locations;

	FThreadSafeCounter StopTaskCounter;
public:
	FDungeonAlg(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells,float NewRadius, TArray<TArray<int32>>& NewNumOfTypes);
	bool IsFinished() const
	{
		return bFinished;
	}
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();

	static FDungeonAlg* JoyInit(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells, float NewRadius, TArray<TArray<int32>>& NewNumOfTypes);

	static void Shutdown();

	static bool IsThreadFinished();

	FVector2D GetRandomPointInCircle();
	FVector2D GetRandomPointInEllipse();
	bool ValueInRange(float Value, float Min, float Max);
	float NormalRand(float Min,float Max);
	void GenerateCells();
	FVector2D SeperateCells(FCells& Cell);
	FVector2D CohaseCells(FCells& Cell);
	void ShortenEdge(FEdgeD& Edge, FCells Cell);
	void ShortenHallways(TArray<FCells>& Rooms, TArray<FCells>& Hallways);
	TArray<FCells> ThickenHallways(TArray<FEdgeD> HallwaysEdges,float Thickness);
	TArray<FCells> ShortenHallway(FCells& Hallway, FCells& Room);
	bool DoCellContains(FCells& Cell, FVector2D Point);
	void DetermineMainHubs();
	TArray<FEdgeD> GenerateGraph();
	TArray<FCells> CreateHallways(TArray<FEdgeD> TreeEdges);
	TArray<FCells> FindHallwaysRooms(TArray<FCells> Hallways);
	bool CellContains(FCells Cell1,FCells Cell2);//Checks if 1 contains 2

	bool DoOverlap(FCells C1,FCells C2);
	FVector2D CentreOfCell(FCells Cell);
	void MoveCell(FVector2D MoveVector,FCells& Cell);
	~FDungeonAlg();
private:
	bool bFinished = false;
	uint32 NumberOfCells;
	float Radius;
	TArray<FCells> Cells;
	TArray<FCells> Hubs;
	TArray<TArray<int32>>* NumOfTypes;
	float HallwayWidth=40.0f;
	float Ellipse_Width, Ellipse_Height;
};

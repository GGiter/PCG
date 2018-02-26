// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonAlg.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"
#include <ctime>
#include <cstdlib>


FDungeonAlg* FDungeonAlg::Runnable = NULL;

FDungeonAlg::FDungeonAlg(TArray<TArray<FVector> >& LocationArray,uint32 NewNumberOfCells,float NewRadius,TArray<TArray<int32>>& NewNumOfTypes)
{
	Locations = &LocationArray;
	NumberOfCells = NewNumberOfCells;
	Radius = NewRadius;
	Thread = FRunnableThread::Create(this, TEXT("FDungeonAlg"), 0, TPri_AboveNormal);
	NumOfTypes = &NewNumOfTypes;
}

bool FDungeonAlg::Init()
{
	//srand(time(NULL));
	
	GenerateCells();
	
	return true;
}

uint32 FDungeonAlg::Run()
{
	FPlatformProcess::Sleep(0.03);
	TArray<FCells> Temp;
	for (FCells Cell : Cells)
	{
		FVector2D Seperation = SeperateCells(Cell);
		FVector2D Cohesion = CohaseCells(Cell);
	//	UE_LOG(LogTemp, Warning, TEXT("Move %s"), *CentreOfCell(Cell).ToString());
		MoveCell(Seperation*13.0f + Cohesion * 2.0f, Cell);

		//MoveCell(MoveVector*2.0f, NewCells[i]);
		Temp.Add(Cell);
	}
	Cells = Temp;
	for (FCells Cell : Hubs)
	{


		//MoveCell(MoveVector*2.0f, NewCells[i]);
	//	UE_LOG(LogTemp, Warning, TEXT("Move2 %s"), *CentreOfCell(Cell).ToString());
	}
	//SeperateCells(Cells);
	DetermineMainHubs();
	TArray<FCells> Hallways = CreateHallways(GenerateGraph());
	TArray<FCells> Rooms =FindHallwaysRooms(Hallways);
	ShortenHallways(Rooms, Hallways);
	
	//SeperateCells(Hubs);
	int32 index = 0;
	TArray<int32> TypesTemp;
	TypesTemp.Add(index);
	for (FCells Hub : Hubs)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Hub.P1,0.0f));
		Temp.Add(FVector(Hub.P2, 0.0f));
		Temp.Add(FVector(Hub.P3, 0.0f));
		Temp.Add(FVector(Hub.P4, 0.0f));
		Locations->Add(Temp);
		index++;
	}
	TypesTemp.Add(index - 1);
	NumOfTypes->Add(TypesTemp);
	TypesTemp.Empty();
	TypesTemp.Add(index);
	for (FCells Room : Rooms)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Room.P1, 0.0f));
		Temp.Add(FVector(Room.P2, 0.0f));
		Temp.Add(FVector(Room.P3, 0.0f));
		Temp.Add(FVector(Room.P4, 0.0f));
		Locations->Add(Temp);
		index++;
	}
	TypesTemp.Add(index - 1);
	NumOfTypes->Add(TypesTemp);
	TypesTemp.Empty();
	TypesTemp.Add(index);
	for (FCells Hallway : Hallways)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Hallway.P1, 0.0f));
		Temp.Add(FVector(Hallway.P2, 0.0f));
		Temp.Add(FVector(Hallway.P3, 0.0f));
		Temp.Add(FVector(Hallway.P4, 0.0f));
		Locations->Add(Temp);
		index++;
	}
	TypesTemp.Add(index - 1);
	NumOfTypes->Add(TypesTemp);
	UE_LOG(LogTemp, Warning, TEXT("Genrera %d"), Locations->Num());
	bFinished = true;
	return 0;
}

void FDungeonAlg::Stop()
{
	StopTaskCounter.Increment();
}

void FDungeonAlg::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

FDungeonAlg * FDungeonAlg::JoyInit(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells, float NewRadius, TArray<TArray<int32>>& NewNumOfTypes)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FDungeonAlg(LocationArray,NewNumberOfCells,NewRadius,NewNumOfTypes);
	}
	return Runnable;
}

void FDungeonAlg::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FDungeonAlg::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

FVector2D FDungeonAlg::GetRandomPointInCircle()
{
	float t = 2.0f*PI*FMath::FRand();
	float u = FMath::FRand()+ FMath::FRand();
	float r;
	if (u > 1.0f)
	{
		r = 2.0f - u;
	}
	else
	{
		r = u;
	}
	return FVector2D(FMath::Abs(2.0f*Radius*r*FMath::Cos(t)),FMath::Abs(2.0f*Radius*r*FMath::Sin(t)));
}

FVector2D FDungeonAlg::GetRandomPointInEllipse()
{
	float t = 2.0f*PI*NormalRand(0.0f, 1.0f);
	float u = NormalRand(0.0f, 1.0f) + NormalRand(0.0f, 1.0f);
	float r;
	if (u > 1.0f)
	{
		r = 2.0f - u;
	}
	else
	{
		r = u;
	}
	return FVector2D(Ellipse_Width*r*FMath::Cos(t)/2.0f, Ellipse_Height*r*FMath::Sin(t)/2.0f);
}

bool FDungeonAlg::ValueInRange(float Value, float Min, float Max)
{
	return (Value >= Min)&&(Value<=Max);
}

float FDungeonAlg::NormalRand(float Min, float Max)
{
	float u1 = 1.0f - FMath::FRand();
	float u2 = 1.0f - FMath::FRand();
	float StdNormal = FMath::Sqrt(-2.0f * FMath::Loge(u1)) * FMath::Sin(2.0f*PI*u2);
	return Min + Max*StdNormal;
}

void FDungeonAlg::GenerateCells()
{
	for (uint32 i = 0; i < NumberOfCells; ++i)
	{
		FVector2D P1 = GetRandomPointInCircle();
		float Length = FMath::FRandRange(Radius / 2.0f, 1.5f*Radius);
		float RadioW = FMath::FRandRange(0.5f, 1.2f), RadioH = FMath::FRandRange(0.5f, 1.2f);
		FVector2D P2(P1.X + Length*RadioW, P1.Y);
		FVector2D P3(P1.X + Length*RadioW, P1.Y + Length*RadioH);
		FVector2D P4(P1.X, P1.Y + Length*RadioH);
		Cells.Add(FCells(P1,P2,P3,P4));
	}
}

FVector2D FDungeonAlg::SeperateCells(FCells& Cell)
{
		FVector2D MoveVector(0.0f, 0.0f);
		int32 NeighbourCount = 0;
		
			for (FCells Cell2 : Cells)
			{
				if (Cells.Find(Cell) != Cells.Find(Cell2))
				{			
						MoveVector += CentreOfCell(Cell2) - CentreOfCell(Cell);
						NeighbourCount++;
				}
			}
			if (NeighbourCount > 0)
			{
				MoveVector /= (NeighbourCount);
				MoveVector *= -1;
				//MoveVector.Normalize(1);
				return MoveVector;
				//UE_LOG(LogTemp, Warning, TEXT("Move %s"), *CentreOfCell(NewCells[i]).ToString());
				//MoveCell(MoveVector*2.0f, NewCells[i]);
				//UE_LOG(LogTemp, Warning, TEXT("Move2 %s"), *CentreOfCell(NewCells[i]).ToString());
			}
			else
				return FVector2D(0.0f, 0.0f);

}

FVector2D FDungeonAlg::CohaseCells(FCells & Cell)
{
	FVector2D MoveVector(0.0f, 0.0f);
	int32 NeighbourCount = 0;
	for (FCells Cell2 : Cells)
	{
		if (Cells.Find(Cell2) != Cells.Find(Cell))
		{
			MoveVector += CentreOfCell(Cell2);
		}
	}
	if(NeighbourCount==0)
		return FVector2D(0.0f,0.0f);
	else
	{
		MoveVector /= NeighbourCount;
		MoveVector -= CentreOfCell(Cell);
		//MoveVector.Normalize();
		return MoveVector;
	}
}

void FDungeonAlg::ShortenEdge(FEdgeD & Edge, FCells Cell)
{
	//Edge.P1 must be a centre of Cell
	if (Edge.P1.X == Edge.P2.X)
	{
		FVector2D Dist(FMath::Abs(Cell.P1.X - Cell.P4.X)/2.0f, FMath::Abs(Cell.P1.Y - Cell.P4.Y)/2.0f);
		if (Edge.P2.Y < Edge.P1.Y)
			Dist *= -1.0f;
		Edge.P1 += Dist;
	}
	else
	{
		FVector2D Dist(FMath::Abs(Cell.P1.X - Cell.P2.X)/2.0f, FMath::Abs(Cell.P1.Y - Cell.P2.Y)/2.0f);
		if (Edge.P2.X < Edge.P1.X)
			Dist *= -1.0f;
		Edge.P1 += Dist;
	}
}

void FDungeonAlg::ShortenHallways(TArray<FCells>& Rooms, TArray<FCells>& Hallways)
{
	/*
	for (int32 i=0;i<Hallways.Num();++i)
	{
		for (int32 j = 0; j < Hallways.Num(); ++j)
		{
			if (i != j)
			{
				if (DoOverlap(Hallways[i], Hallways[j]))
				{
					Hallways.Append(ShortenHallway(Hallways[i], Hallways[j]));
					Hallways.RemoveAt(i);
					break;
				}
			}
		}
	}
	
	*/
		TArray<FCells> Temp;
		for (FCells Hallway1 : Hallways)
		{
			bool check = true;
			for (FCells Hallway2 : Hallways)
			{
				if (Hallways.Find(Hallway1) != Hallways.Find(Hallway2))
				{
					if (CellContains(Hallway2,Hallway1))
					{
						check = false;
						break;
					}
				}

			}
			if (check)
				Temp.Add(Hallway1);
		}

		Hallways = Temp;
	
	
	
	TArray<FCells> NewHallways;
	for (FCells Hallway : Hallways)
	{
		for (FCells Room : Rooms)
		{
			if (DoOverlap(Room,Hallway))
			{

				NewHallways.Append(ShortenHallway(Hallway, Room));
			}
		
		}
	}
	Hallways = NewHallways;
	Temp.Empty();
	for (FCells Hallway1 : Hallways)
	{
		bool check = true;
		for (FCells Hallway2 : Hallways)
		{
			if (Hallways.Find(Hallway1) != Hallways.Find(Hallway2))
			{
				if (CellContains(Hallway2, Hallway1))
				{
					check = false;
					break;
				}
			}

		}
		if (check)
			Temp.Add(Hallway1);
	}

	Hallways = Temp;
}

TArray<FCells> FDungeonAlg::ThickenHallways(TArray<FEdgeD> HallwaysEdges, float Thickness)
{
	TArray<FCells> Hallways;
	for (FEdgeD Edge : HallwaysEdges)
	{
		if (Edge.P1.X == Edge.P2.X)
		{
			Hallways.Add(FCells(FVector2D(Edge.P1.X + -0.5f*Thickness, Edge.P1.Y), FVector2D(Edge.P1.X + 0.5f*Thickness, Edge.P1.Y), FVector2D(Edge.P2.X + 0.5f*Thickness, Edge.P2.Y), FVector2D(Edge.P2.X + -0.5f*Thickness, Edge.P2.Y)));
		}
		else if (Edge.P1.Y == Edge.P2.Y)
		{
			if(Edge.P1.X<Edge.P2.X)
			Hallways.Add(FCells(FVector2D(Edge.P1.X, Edge.P1.Y + -0.5f*Thickness),FVector2D(Edge.P2.X, Edge.P2.Y + -0.5f*Thickness), FVector2D(Edge.P2.X, Edge.P2.Y + 0.5f*Thickness), FVector2D(Edge.P1.X, Edge.P1.Y + 0.5f*Thickness)));
			else
				Hallways.Add(FCells(FVector2D(Edge.P2.X, Edge.P2.Y + -0.5f*Thickness), FVector2D(Edge.P1.X, Edge.P1.Y + -0.5f*Thickness), FVector2D(Edge.P1.X, Edge.P1.Y + 0.5f*Thickness), FVector2D(Edge.P2.X, Edge.P2.Y + 0.5f*Thickness)));
		}
		else
		{
			FVector2D Start = Edge.P1;
			FVector2D End = Edge.P2;
			FVector2D Dir = End - Start;
			FVector2D Perp(Dir.X, -Dir.Y);
			Perp.Normalize();
			Dir.Normalize();
			FVector2D POffset = Perp * Thickness *0.5f;
			FVector2D DOffset = Dir * Thickness *0.5f;
			Hallways.Add(FCells(FVector2D(Start + POffset - DOffset), FVector2D(Start - POffset - DOffset), FVector2D(End + POffset + DOffset), FVector2D(End - POffset + DOffset)));
		}

	}
	return Hallways;
}

TArray<FCells> FDungeonAlg::ShortenHallway(FCells & Hallway, FCells & Room)
{
	TArray<FCells> NewHallways;
	
	if (Hallway.Width==90.0f)
	{
		if (DoCellContains(Room, Hallway.P1) || DoCellContains(Room, Hallway.P2))
		{
			NewHallways.Add(FCells(FVector2D(Hallway.P1.X, Room.P4.Y) + FVector2D(0.0f,0.0f), FVector2D(Hallway.P2.X, Room.P3.Y) + FVector2D(0.0f, 0.0f), Hallway.P3 - FVector2D(0.0f, 0.0f), Hallway.P4 -FVector2D(0.0f, 0.0f)));
		}
		else if (DoCellContains(Room, Hallway.P3) || DoCellContains(Room, Hallway.P4))
		{
			NewHallways.Add(FCells(Hallway.P1 + FVector2D(0.0f, 0.0f), Hallway.P2 + FVector2D(0.0f, 0.0f), FVector2D(Hallway.P2.X, Room.P2.Y) -FVector2D(0.0f,0.0f), FVector2D(Hallway.P1.X, Room.P1.Y) - FVector2D(0.0f, 0.0f)));
		}
		else
		{
			NewHallways.Add(FCells(FVector2D(Hallway.P1.X, Room.P4.Y) + FVector2D(0.0f, 0.0f), FVector2D(Hallway.P2.X, Room.P3.Y) + FVector2D(0.0f, 0.0f), Hallway.P3 - FVector2D(0.0f, 0.0f), Hallway.P4 - FVector2D(0.0f, 0.0f)));
			NewHallways.Add(FCells(Hallway.P1 + FVector2D(0.0f, 0.0f), Hallway.P2 + FVector2D(0.0f, 0.0f), FVector2D(Hallway.P2.X, Room.P2.Y) - FVector2D(0.0f, 0.0f), FVector2D(Hallway.P1.X, Room.P1.Y) - FVector2D(0.0f, 0.0f)));
		}
	}
	else
	{
		if (DoCellContains(Room, Hallway.P2) || DoCellContains(Room, Hallway.P3))
		{
			NewHallways.Add(FCells(Hallway.P1 + FVector2D(0.0f, 0.0f), FVector2D(Room.P1.X, Hallway.P1.Y) - FVector2D(0.0f, 0.0f), FVector2D(Room.P4.X, Hallway.P3.Y) + FVector2D(0.0f, 0.0f), Hallway.P4 - FVector2D(0.0f, 0.0f)));
		}
		else if (DoCellContains(Room, Hallway.P1) || DoCellContains(Room, Hallway.P4))
		{
			NewHallways.Add(FCells(FVector2D(Room.P2.X, Hallway.P2.Y) + FVector2D(0.0f, 0.0f), Hallway.P2 - FVector2D(0.0f, 0.0f), Hallway.P3 + FVector2D(0.0f, 0.0f), FVector2D(Room.P3.X, Hallway.P3.Y) - FVector2D(0.0f, 0.0f)));
		}
		else
		{
			NewHallways.Add(FCells(Hallway.P1+FVector2D(0.0f,0.0f), FVector2D(Room.P1.X,Hallway.P2.Y) - FVector2D(0.0f, 0.0f), FVector2D(Room.P4.X, Hallway.P3.Y) + FVector2D(0.0f, 0.0f), Hallway.P4 - FVector2D(0.0f, 0.0f)));
			NewHallways.Add(FCells(FVector2D(Room.P2.X, Hallway.P1.Y) + FVector2D(0.0f, 0.0f), Hallway.P2 - FVector2D(0.0f, 0.0f), Hallway.P3 + FVector2D(0.0f, 0.0f), FVector2D(Room.P3.X, Hallway.P4.Y) - FVector2D(0.0f, 0.0f)));
		}
	}

	return NewHallways;
}

bool FDungeonAlg::DoCellContains(FCells & Cell, FVector2D Point)
{
	FVector2D Centre = CentreOfCell(Cell);
	if ((Point.X <= Centre.X + 0.5f*Cell.Width) && (Point.X >= Centre.X - 0.5f*Cell.Width) &&
		(Point.Y <= Centre.Y + 0.5f*Cell.Height) && (Point.Y >= Centre.Y - 0.5f*Cell.Height))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void FDungeonAlg::DetermineMainHubs()
{

	for (FCells Cell : Cells)
	{
		if (FVector2D::Distance(Cell.P1, Cell.P4) >= Radius * 1.2f&&FVector2D::Distance(Cell.P1, Cell.P2) >= Radius*1.2f)
		{
			Hubs.Add(Cell);
			//Cells.Remove(Cell);
		}
	}
	if (Hubs.Num() == 0)
	{
		Hubs.Add(Cells[0]);
		Hubs.Add(Cells[Cells.Num() - 1]);
	}
}

TArray<FEdgeD> FDungeonAlg::GenerateGraph()
{
	
	TArray<FVector2D> Hubs2D;
	for(FCells Hub : Hubs)
	{
		Hubs2D.Add(FVector2D(CentreOfCell(Hub).X, CentreOfCell(Hub).Y));
	} 
	Delaunay DelaunayAlg;
	TArray<FTriangle> Triangles = DelaunayAlg.Triangulate(Hubs2D);
	TArray<FEdgeD> Edges = DelaunayAlg.GetEdges();
	TArray<FVector2D> Vertices = DelaunayAlg.GetVertices();
	
	Edges.Sort([this](const FEdgeD& A, const FEdgeD& B) {
		return FVector2D::Distance(A.P1,A.P2)<FVector2D::Distance(B.P1,B.P2);
	});
	TArray<FEdgeD> TreeEdges;
	TArray<FVector2D> ControlCheck;
	for (FEdgeD Edge : Edges)
	{
		if (TreeEdges.Find(Edge)==INDEX_NONE&&FVector2D::Distance(Edge.P1,Edge.P2)>Radius/2.0f)
		{
			TreeEdges.AddUnique(Edge);
			ControlCheck.AddUnique(Edge.P1);
			ControlCheck.AddUnique(Edge.P2);
		}
	}
	return TreeEdges;
}

TArray<FCells> FDungeonAlg::CreateHallways(TArray<FEdgeD> TreeEdges)
{
	TArray<FEdgeD> HallwaysEdges;
	
	for (FEdgeD Edge : TreeEdges)
	{
		if (Edge.P1.X==Edge.P2.X)
		{
			HallwaysEdges.Add(Edge);
		}
		else if (Edge.P1.Y==Edge.P2.Y)
		{
			HallwaysEdges.Add(Edge);
		}
		else
		{	
			float Width = 90.0f;
			if (FMath::FRand()<=0.5f)
			{
				if (Edge.P1.Y < Edge.P2.Y)
				{
					HallwaysEdges.Add(FEdgeD(Edge.P1, FVector2D(Edge.P1.X, Edge.P2.Y - Width / 1.0f)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P1.X, Edge.P2.Y), Edge.P2));
				}
				else
				{
					HallwaysEdges.Add(FEdgeD(Edge.P2, FVector2D(Edge.P2.X, Edge.P1.Y - Width / 1.0f)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P2.X, Edge.P1.Y), Edge.P1));
				}
				
			}
			else
			{
				if (Edge.P1.Y < Edge.P2.Y)
				{
					HallwaysEdges.Add(FEdgeD(Edge.P1, FVector2D(Edge.P2.X - Width / 1.0f, Edge.P1.Y)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P2.X, Edge.P1.Y), Edge.P2));
				}
				else
				{
					HallwaysEdges.Add(FEdgeD(Edge.P2, FVector2D(Edge.P1.X - Width / 1.0f, Edge.P2.Y)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P1.X, Edge.P2.Y), Edge.P1));
				}
				
			}
		}
	}
	TArray<FEdgeD> Hallways;
	TArray<FCells> Cells= ThickenHallways(HallwaysEdges, 90.0f);
	/*
	for (FEdgeD Edge : HallwaysEdges)
	{
		for (FCells Hub : Hubs)
		{
			if (DoCellContains(Hub,Edge.P1))
			{
				ShortenHallway(Cells[Fi])
				ShortenEdge(Edge, Hub);
				Hallways.Add(Edge);
			}
			else if (DoCellContains(Hub, Edge.P2))
			{
				FEdgeD Temp(Edge.P2, Edge.P1);
				ShortenEdge(Temp, Hub);
				Hallways.Add(FEdgeD(Temp.P2, Temp.P1));
			}
		}
	}
	*/
	TArray<FCells> Ans;
	for (FCells Cell : Cells)
	{
		for (FCells Hub : Hubs)
		{
			if (DoOverlap(Hub, Cell))
			{
				Ans.Append(ShortenHallway(Cell, Hub));
			}
		}
	}
	//return ThickenHallways(Hallways,90.0f);
	return Ans;
}

TArray<FCells> FDungeonAlg::FindHallwaysRooms(TArray<FCells> Hallways)
{
	TArray<FCells> Rooms;
	for (FCells Hallway : Hallways)
	{
		for (FCells Cell : Cells)
		{
			if (DoOverlap(Cell,Hallway)&&Hubs.Find(Cell)==INDEX_NONE)
			{
				Rooms.Add(Cell);
			}

		}
	}
	return Rooms;
}

bool FDungeonAlg::CellContains(FCells Cell1, FCells Cell2)
{
	if (DoCellContains(Cell1,Cell2.P1)&& DoCellContains(Cell1, Cell2.P2)&& DoCellContains(Cell1, Cell2.P3)&& DoCellContains(Cell1, Cell2.P4))
		return true;
	else
		return false;
}

bool FDungeonAlg::DoOverlap(FCells C1, FCells C2)
{
	FVector2D R1 = CentreOfCell(C1);
	FVector2D R2 = CentreOfCell(C2);
	bool xOverlap = ValueInRange(R1.X, R2.X, R2.X + FVector2D::Distance(C2.P1, C2.P2)) || ValueInRange(R2.X, R1.X, R1.X + FVector2D::Distance(C1.P1, C1.P2));
	bool yOverlap = ValueInRange(R1.Y, R2.Y, R2.Y + FVector2D::Distance(C2.P1, C2.P4)) || ValueInRange(R2.Y, R1.Y, R1.Y + FVector2D::Distance(C1.P1, C1.P4));

	//return xOverlap && yOverlap;
	return DoCellContains(C1, C2.P1) || DoCellContains(C1, C2.P2) || DoCellContains(C1, C2.P3) || DoCellContains(C1, C2.P4) || DoCellContains(C2, C1.P1) || DoCellContains(C2, C1.P2) || DoCellContains(C2, C1.P3) || DoCellContains(C2, C1.P4);
}

FVector2D FDungeonAlg::CentreOfCell(FCells Cell)
{
	return FVector2D((Cell.P1.X+ Cell.P2.X + Cell.P3.X + Cell.P4.X)/4.0f, (Cell.P1.Y + Cell.P2.Y + Cell.P3.Y + Cell.P4.Y) / 4.0f);
}

void FDungeonAlg::MoveCell(FVector2D MoveVector, FCells& Cell)
{
	Cell.P1 += MoveVector;
	Cell.P2 += MoveVector;
	Cell.P3 += MoveVector;
	Cell.P4 += MoveVector;
}



FDungeonAlg::~FDungeonAlg()
{
	delete Thread;
	Thread = nullptr;
}

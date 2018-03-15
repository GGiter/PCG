// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonAlg.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"



FDungeonAlg* FDungeonAlg::Runnable = NULL;

FDungeonAlg::FDungeonAlg(TArray<TArray<FVector> >& LocationArray,uint32 NewNumberOfCells,float NewRadius,TArray<TArray<int32>>& NewNumOfTypes, TArray<TArray<FEdge> >& NewRoomsBorders, TArray<TArray<FEdge> >& NewHubsBorders, float NewHallwayWidth)
{
	Locations = &LocationArray;
	RoomsBorders = &NewRoomsBorders;
	HubsBorders = &NewHubsBorders;
	NumberOfCells = NewNumberOfCells;
	Radius = NewRadius;
	HallwayWidth = NewHallwayWidth;
	Thread = FRunnableThread::Create(this, TEXT("FDungeonAlg"), 0, TPri_AboveNormal);
	NumOfTypes = &NewNumOfTypes;
}

bool FDungeonAlg::Init()
{
	double Secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 Seed = (int32)(((int64)Secs) % INT_MAX);
	FMath::RandInit(Seed);
	GenerateCells();
	
	return true;
}

uint32 FDungeonAlg::Run()
{
	FPlatformProcess::Sleep(0.03);
	UE_LOG(LogTemp, Warning, TEXT("Starting Genrera"));
	TArray<FBox2D> Temp;
	for (FBox2D Cell : Cells)
	{
		FVector2D Seperation = SeperateCells(Cell);
		FVector2D Cohesion = CohaseCells(Cell);
	//	UE_LOG(LogTemp, Warning, TEXT("Move %s"), *CentreOfCell(Cell).ToString());

		Cell=Cell.ShiftBy(Seperation*30.0f + Cohesion * 5.0f);
		//MoveCell(MoveVector*2.0f, NewCells[i]);
		Temp.Add(Cell);
	}
	Cells = Temp;
	for (FBox2D Cell : Hubs)
	{


		//MoveCell(MoveVector*2.0f, NewCells[i]);
	//	UE_LOG(LogTemp, Warning, TEXT("Move2 %s"), *CentreOfCell(Cell).ToString());
	}
	//SeperateCells(Cells);
	DetermineMainHubs();
	TArray<FBox2D> Rooms;
	TArray<FBox2D> Hallways = CreateHallways(GenerateGraph(),Rooms);
	
	Rooms.Append(FindHallwaysRooms(Hallways));
	UE_LOG(LogTemp, Warning, TEXT("Genrera %d"), Hubs.Num());
	//ShortenHallways(Rooms, Hallways);

	//SeperateCells(Hubs);
	int32 index = 0;
	TArray<int32> TypesTemp;
	TypesTemp.Add(index);
	for (FBox2D Hub : Hubs)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Hub.Min,0.0f));
		Temp.Add(FVector(FVector2D(Hub.Max.X,Hub.Min.Y), 0.0f));
		Temp.Add(FVector(Hub.Max, 0.0f));
		Temp.Add(FVector(FVector2D(Hub.Min.X, Hub.Max.Y), 0.0f));
		Locations->Add(Temp);
		index++;
	}
	TypesTemp.Add(index - 1);
	NumOfTypes->Add(TypesTemp);
	TypesTemp.Empty();

	
	TypesTemp.Add(index);
	
	for (FBox2D Room : Rooms)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Room.Min, 0.0f));
		Temp.Add(FVector(FVector2D(Room.Max.X, Room.Min.Y), 0.0f));
		Temp.Add(FVector(Room.Max, 0.0f));
		Temp.Add(FVector(FVector2D(Room.Min.X, Room.Max.Y), 0.0f));
		Locations->Add(Temp);
		index++;
	}
	TypesTemp.Add(index - 1);
	NumOfTypes->Add(TypesTemp);
	TypesTemp.Empty();
	
	TypesTemp.Add(index);
	
	for (FBox2D Hallway : Hallways)
	{
		TArray<FVector> Temp;
		Temp.Add(FVector(Hallway.Min, 0.0f));
		Temp.Add(FVector(FVector2D(Hallway.Max.X, Hallway.Min.Y), 0.0f));
		Temp.Add(FVector(Hallway.Max, 0.0f));
		Temp.Add(FVector(FVector2D(Hallway.Min.X, Hallway.Max.Y), 0.0f));
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

FDungeonAlg * FDungeonAlg::JoyInit(TArray<TArray<FVector> >& LocationArray, uint32 NewNumberOfCells, float NewRadius, TArray<TArray<int32>>& NewNumOfTypes, TArray<TArray<FEdge> >& NewRoomsBorders, TArray<TArray<FEdge> >& NewHubsBorders, float NewHallwayWidth)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FDungeonAlg(LocationArray,NewNumberOfCells,NewRadius,NewNumOfTypes,NewRoomsBorders,NewHubsBorders,NewHallwayWidth);
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
		
		Cells.Add(FBox2D(P1,P3));
	}
}

FVector2D FDungeonAlg::SeperateCells(FBox2D& Cell)
{
		FVector2D MoveVector(0.0f, 0.0f);
		int32 NeighbourCount = 0;
		
			for (FBox2D Cell2 : Cells)
			{
				if (Cells.Find(Cell) != Cells.Find(Cell2))
				{			
						MoveVector += Cell2.GetCenter() - Cell.GetCenter();
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

FVector2D FDungeonAlg::CohaseCells(FBox2D & Cell)
{
	FVector2D MoveVector(0.0f, 0.0f);
	int32 NeighbourCount = 0;
	for (FBox2D Cell2 : Cells)
	{
		if (Cells.Find(Cell2) != Cells.Find(Cell))
		{
			MoveVector += Cell2.GetCenter();
		}
	}
	if(NeighbourCount==0)
		return FVector2D(0.0f,0.0f);
	else
	{
		MoveVector /= NeighbourCount;
		MoveVector -= Cell.GetCenter();
		//MoveVector.Normalize();
		return MoveVector;
	}
}

void FDungeonAlg::ShortenEdge(FEdgeD & Edge, FBox2D Cell)
{
	//Edge.P1 must be a centre of Cell
	if (Edge.P1.X == Edge.P2.X)
	{
		FVector2D Dist(FMath::Abs(Cell.Min.X - Cell.Min.X)/2.0f, FMath::Abs(Cell.Min.Y - Cell.Max.Y)/2.0f);
		if (Edge.P2.Y < Edge.P1.Y)
			Dist *= -1.0f;
		Edge.P1 += Dist;
	}
	else
	{
		FVector2D Dist(FMath::Abs(Cell.Min.X - Cell.Max.X)/2.0f, FMath::Abs(Cell.Min.Y - Cell.Min.Y)/2.0f);
		if (Edge.P2.X < Edge.P1.X)
			Dist *= -1.0f;
		Edge.P1 += Dist;
	}
}

void FDungeonAlg::ShortenHallways(TArray<FBox2D>& Rooms, TArray<FBox2D>& Hallways)
{
	TArray<FBox2D> Ans;
	TArray<FBox2D> ToRemove;
	for (int32 i=0;i<Rooms.Num();++i)
	{
		for (int32 j=0;j<Hallways.Num();++j)
		{
			if (Rooms[i].Intersect(Hallways[j]))
			{		
				//Ans.Append(ShortenHallway(Hallways[j], Rooms[i]));	
				//Hallways.Append(ShortenHallway(Hallways[j], Rooms[i]));
				ToRemove.Add(Hallways[j]);
			}
		}
		for (int32 j = 0; j < ToRemove.Num();++j)
		{
			Hallways.Remove(ToRemove[j]);
		}
	}
	//Hallways = Ans;
	Ans.Empty();
	for (FBox2D Hallway : Hallways)
	{
		if (Hallway.GetSize().X < 80.0f || Hallway.GetSize().Y < 80.0f)
		{

		}
		else
		{
			Ans.Add(Hallway);
		}
	}
	Hallways = Ans;
}

TArray<FBox2D> FDungeonAlg::ThickenHallways(TArray<FEdgeD> HallwaysEdges, float Thickness)
{
	TArray<FBox2D> Hallways;
	for (FEdgeD Edge : HallwaysEdges)
	{
		if (Edge.P1.X == Edge.P2.X)
		{
			if(Edge.P1.Y<Edge.P2.Y)
			Hallways.Add(FBox2D(FVector2D(Edge.P1.X + -0.5f*Thickness, Edge.P1.Y),FVector2D(Edge.P2.X + 0.5f*Thickness, Edge.P2.Y)));
			else
			Hallways.Add(FBox2D(FVector2D(Edge.P2.X + -0.5f*Thickness, Edge.P2.Y), FVector2D(Edge.P1.X + 0.5f*Thickness, Edge.P1.Y)));
		}
		else if (Edge.P1.Y == Edge.P2.Y)
		{
			if(Edge.P1.X<Edge.P2.X)
			Hallways.Add(FBox2D(FVector2D(Edge.P1.X, Edge.P1.Y + -0.5f*Thickness), FVector2D(Edge.P2.X, Edge.P2.Y + 0.5f*Thickness)));
			else
				Hallways.Add(FBox2D(FVector2D(Edge.P2.X, Edge.P2.Y + -0.5f*Thickness), FVector2D(Edge.P1.X , Edge.P1.Y + 0.5f*Thickness)));
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
			Hallways.Add(FBox2D(FVector2D(Start + POffset - DOffset), FVector2D(End + POffset + DOffset)));
		}

	}
	return Hallways;
}

TArray<FBox2D> FDungeonAlg::ShortenHallway(FBox2D & Hallway, FBox2D & Room,TArray<FEdge>& Borders)
{
	TArray<FBox2D> NewHallways;
	
	
		if (Room.IsInside(Hallway.Min) && Room.IsInside(FVector2D(Hallway.Max.X, Hallway.Min.Y)))
		{
			NewHallways.Add(FBox2D(FVector2D(Hallway.Min.X, Room.Max.Y) , Hallway.Max));
			Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Max.Y, 100.0f), FVector(Hallway.Max.X, Room.Max.Y, 100.0f)));
		}
		else if (Room.IsInside(Hallway.Max) && Room.IsInside(FVector2D(Hallway.Min.X, Hallway.Max.Y)))
		{
			NewHallways.Add(FBox2D(Hallway.Min, FVector2D(Hallway.Max.X, Room.Min.Y)));
			Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Min.Y, 100.0f), FVector(Hallway.Max.X, Room.Min.Y, 100.0f)));
		}	
		else if (Room.IsInside(FVector2D(Hallway.Max.X, Hallway.Min.Y)) && Room.IsInside(Hallway.Max))
		{
			NewHallways.Add(FBox2D(Hallway.Min , FVector2D(Room.Min.X, Hallway.Max.Y)));
			Borders.Add(FEdge(FVector(Room.Min.X, Hallway.Min.Y, 100.0f), FVector(Room.Min.X, Hallway.Max.Y, 100.0f)));
		}
		else if (Room.IsInside(Hallway.Min) && Room.IsInside(FVector2D(Hallway.Min.X,Hallway.Max.Y)))
		{
			NewHallways.Add(FBox2D(FVector2D(Room.Max.X, Hallway.Min.Y) , Hallway.Max ));
			Borders.Add(FEdge(FVector(Room.Max.X, Hallway.Min.Y, 100.0f), FVector(Room.Max.X, Hallway.Max.Y, 100.0f)));
		}
		else if (Room.IsInside(Hallway.Min) || Room.IsInside(FVector2D(Hallway.Max.X, Hallway.Min.Y)))
		{
			NewHallways.Add(FBox2D(FVector2D(Hallway.Min.X, Room.Max.Y), Hallway.Max));
			Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Max.Y, 100.0f), FVector(Hallway.Max.X, Room.Max.Y, 100.0f)));
		}
		else if (Room.IsInside(Hallway.Max) || Room.IsInside(FVector2D(Hallway.Min.X, Hallway.Max.Y)))
		{
			NewHallways.Add(FBox2D(Hallway.Min, FVector2D(Hallway.Max.X, Room.Min.Y)));
			Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Min.Y, 100.0f), FVector(Hallway.Max.X, Room.Min.Y, 100.0f)));
		}
		else if (Room.IsInside(FVector2D(Hallway.Max.X, Hallway.Min.Y)) || Room.IsInside(Hallway.Max))
		{
			NewHallways.Add(FBox2D(Hallway.Min, FVector2D(Room.Min.X, Hallway.Max.Y)));
			Borders.Add(FEdge(FVector(Room.Min.X, Hallway.Min.Y, 100.0f), FVector(Room.Min.X, Hallway.Max.Y, 100.0f)));
		}
		else if (Room.IsInside(Hallway.Min) || Room.IsInside(FVector2D(Hallway.Min.X, Hallway.Max.Y)))
		{
			NewHallways.Add(FBox2D(FVector2D(Room.Max.X, Hallway.Min.Y), Hallway.Max));
			Borders.Add(FEdge(FVector(Room.Max.X, Hallway.Min.Y, 100.0f), FVector(Room.Max.X, Hallway.Max.Y, 100.0f)));
		}
		else 
		{
			if (Hallway.GetSize().Y == HallwayWidth)
			{
				NewHallways.Add(FBox2D(Hallway.Min, FVector2D(Room.Min.X, Hallway.Max.Y)));
				Borders.Add(FEdge(FVector(Room.Min.X, Hallway.Min.Y, 100.0f), FVector(Room.Min.X, Hallway.Max.Y, 100.0f)));
				NewHallways.Add(FBox2D(FVector2D(Room.Max.X, Hallway.Min.Y), Hallway.Max));
				Borders.Add(FEdge(FVector(Room.Max.X, Hallway.Min.Y, 100.0f), FVector(Room.Max.X, Hallway.Max.Y, 100.0f)));
			}
			else
			{
				NewHallways.Add(FBox2D(Hallway.Min, FVector2D(Hallway.Max.X, Room.Min.Y)));
				Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Min.Y, 100.0f), FVector(Hallway.Max.X, Room.Min.Y, 100.0f)));
				NewHallways.Add(FBox2D(FVector2D(Hallway.Min.X, Room.Max.Y), Hallway.Max));
				Borders.Add(FEdge(FVector(Hallway.Min.X, Room.Max.Y, 100.0f), FVector(Hallway.Max.X, Room.Max.Y, 100.0f)));
			}
			
			
		}
	
	
	return NewHallways;
}

bool FDungeonAlg::DoCellContains(FBox2D & Cell, FVector2D Point)
{
	FVector2D Centre = Cell.GetCenter();
	if ((Point.X <= Centre.X + 0.5f*Cell.GetSize().X) && (Point.X >= Centre.X - 0.5f*Cell.GetSize().X) &&
		(Point.Y <= Centre.Y + 0.5f*Cell.GetSize().Y) && (Point.Y >= Centre.Y - 0.5f*Cell.GetSize().Y))
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

	for (FBox2D Cell : Cells)
	{
		if (Cell.GetSize().X >= Radius * 1.2f&&Cell.GetSize().Y >= Radius*1.2f&&Hubs.Num()<6)
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
	for(FBox2D Hub : Hubs)
	{
		Hubs2D.Add(Hub.GetCenter());
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

TArray<FBox2D> FDungeonAlg::CreateHallways(TArray<FEdgeD> TreeEdges,TArray<FBox2D>& Rooms)
{
	TArray<FEdgeD> HallwaysEdges;
	
	for (FEdgeD Edge : TreeEdges)
	{
		//Edge = FEdgeD(FVector2D(FMath::RoundToFloat(Edge.P1.X), FMath::RoundToFloat(Edge.P1.Y)), FVector2D(FMath::RoundToFloat(Edge.P2.X), FMath::RoundToFloat(Edge.P2.Y)));
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
			float Width = HallwayWidth;
			if (FMath::FRand()<=0.5f)
			{
				if (Edge.P1.Y < Edge.P2.Y&&Edge.P1.X<Edge.P2.X)
				{
					
					HallwaysEdges.Add(FEdgeD(Edge.P1, FVector2D(Edge.P1.X, Edge.P2.Y - Width / 2.0f)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P1.X + Width / 2.0f, Edge.P2.Y), Edge.P2));
					Rooms.Add(FBox2D(FVector2D(Edge.P1.X-Width/2.0f, Edge.P2.Y - Width/2.0f), FVector2D(Edge.P1.X+Width/2.0f, Edge.P2.Y + Width/2.0f)));
					TArray<FEdge> Borders;
					Borders.Add(FEdge(FVector(Edge.P1.X - Width / 2.0f, Edge.P2.Y - Width / 2.0f, 100.0f), FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y - Width / 2.0f, 100.0f)));
					Borders.Add(FEdge(FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y - Width / 2.0f, 100.0f), FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y + Width / 2.0f, 100.0f)));
					RoomsBorders->Add(Borders);
					
				}
				else if(Edge.P1.X<Edge.P2.X)
				{
					
					HallwaysEdges.Add(FEdgeD(Edge.P2, FVector2D(Edge.P2.X, Edge.P1.Y-Width / 2.0f)));
					HallwaysEdges.Add(FEdgeD(Edge.P1,FVector2D(Edge.P2.X - Width / 2.0f, Edge.P1.Y)));
					Rooms.Add(FBox2D(FVector2D(Edge.P2.X - Width / 2.0f, Edge.P1.Y - Width / 2.0f), FVector2D(Edge.P2.X + Width / 2.0f, Edge.P1.Y + Width / 2.0f)));
					TArray<FEdge> Borders;
					Borders.Add(FEdge(FVector(Edge.P2.X - Width / 2.0f, Edge.P1.Y - Width / 2.0f,100.0f), FVector(Edge.P2.X + Width / 2.0f, Edge.P1.Y - Width / 2.0f, 100.0f)));
					Borders.Add(FEdge(FVector(Edge.P2.X - Width / 2.0f, Edge.P1.Y - Width / 2.0f, 100.0f), FVector(Edge.P2.X + Width / 2.0f, Edge.P1.Y + Width / 2.0f, 100.0f)));
					RoomsBorders->Add(Borders);
					
				}
				
			}
			else
			{
				if (Edge.P1.Y < Edge.P2.Y&&Edge.P1.X<Edge.P2.X)
				{
					
					HallwaysEdges.Add(FEdgeD(Edge.P1, FVector2D(Edge.P2.X - Width / 2.0f, Edge.P1.Y)));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P2.X, Edge.P1.Y+Width / 2.0f), Edge.P2));
					Rooms.Add(FBox2D(FVector2D(Edge.P2.X - Width / 2.0f, Edge.P1.Y - Width / 2.0f), FVector2D(Edge.P2.X+Width/2.0f,Edge.P1.Y+ Width / 2.0f)));
					TArray<FEdge> Borders;
					Borders.Add(FEdge(FVector(Rooms[Rooms.Num()-1].Min,100.0f), FVector(Rooms[Rooms.Num() - 1].Min.X, Rooms[Rooms.Num() - 1].Max.Y, 100.0f)));
					Borders.Add(FEdge(FVector(Rooms[Rooms.Num() - 1].Min.X, Rooms[Rooms.Num() - 1].Max.Y, 100.0f), FVector(Rooms[Rooms.Num() - 1].Max, 100.0f)));
					RoomsBorders->Add(Borders);
					
				}
				else if (Edge.P1.X<Edge.P2.X)
				{
					
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P1.X, Edge.P2.Y + Width / 2.0f),Edge.P1));
					HallwaysEdges.Add(FEdgeD(FVector2D(Edge.P1.X + Width / 2.0f, Edge.P2.Y), Edge.P2));
					Rooms.Add(FBox2D(FVector2D(Edge.P1.X - Width / 2.0f, Edge.P2.Y - Width / 2.0f), FVector2D(Edge.P1.X + Width / 2.0f, Edge.P2.Y + Width / 2.0f)));
					TArray<FEdge> Borders;
					Borders.Add(FEdge(FVector(Edge.P1.X - Width / 2.0f, Edge.P2.Y + Width / 2.0f, 100.0f), FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y+ Width / 2.0f, 100.0f)));
					Borders.Add(FEdge(FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y - Width / 2.0f, 100.0f), FVector(Edge.P1.X + Width / 2.0f, Edge.P2.Y + Width / 2.0f, 100.0f)));
					RoomsBorders->Add(Borders);
					
					
					
				}
				
			}
		}
	}
	TArray<FBox2D> RoomsTemp;
	TArray<FBox2D> Cells = ThickenHallways(HallwaysEdges, HallwayWidth);
	int32 count = 0, k = 0;
	
	for (int32 i = 0; i < Cells.Num(); ++i)
	{
		for (int32 j = 0; j < Cells.Num(); ++j)
		{
			if (i != j)
			{
				if (Cells[i].Intersect(Cells[j]))
				{
					count++;
				}
			}
		}
		if (count > 3)
		{
			Cells.RemoveAt(i);
			i = 0;
		}
		count = 0;
	}
	count = 0;
	for (int32 i = 0; i < Rooms.Num(); ++i)
	{
		bool Check = false;
		for (int32 j = 0; j < Cells.Num(); ++j)
		{
			if (Cells[j].IsInside(Rooms[i].Min)&& Cells[j].IsInside(FVector2D(Rooms[i].Max.X, Rooms[i].Min.Y))&& Cells[j].IsInside(FVector2D(Rooms[i].Min.X,Rooms[i].Max.Y))&& Cells[j].IsInside(Rooms[i].Max))
			{
				Check = true;
			}
			
		}
		if (!Check)
		{
			RoomsTemp.Add(Rooms[i]);
			
		}
		else
		{
			RoomsBorders->RemoveAt(i - k);
			k++;
		}
			
	}
	Rooms = RoomsTemp;
	TArray<FBox2D> Hallways;
	
	
	TArray<FBox2D> Ans, Temp;
	for (FBox2D Hub : Hubs)
	{
		TArray<FEdge> Borders;
		for (FBox2D Cell : Cells)
		{
			if (Cell.Intersect(Hub))
			{
				Temp.Append(ShortenHallway(Cell, Hub, Borders));
				Ans.Append(Temp);
			}
		}
		HubsBorders->Add(Borders);
		Temp.Empty();
	}
	return Ans;
}

TArray<FBox2D> FDungeonAlg::FindHallwaysRooms(TArray<FBox2D>& Hallways)
{
	TArray<FBox2D> Rooms;
	TArray<FBox2D> Ans, HallwayTemp;
	for (FBox2D Hallway : Hallways)
	{
		for (FBox2D Cell : Cells)
		{
			if (Cell.Intersect(Hallway)&&Hubs.Find(Cell)==INDEX_NONE&&Rooms.Find(Cell)==INDEX_NONE)
			{
				if (Hallway.GetSize().X == HallwayWidth)
				{
					if (Hallway.Min.X>Cell.Min.X&&Hallway.Max.X<Cell.Max.X)
					{
						Rooms.Add(Cell);
						HallwayTemp.Add(Cell);
					}
				}
				else
				{
					if (Hallway.Min.Y>Cell.Min.Y&&Hallway.Max.Y<Cell.Max.Y)
					{
						Rooms.Add(Cell);
						HallwayTemp.Add(Cell);
					}
				}
				
				
			}
			
		}
		if (HallwayTemp.Num() > 0)
		{
			Ans.Append(BuildHallways(HallwayTemp, Hallway));
		}
		else
		{
			Ans.Add(Hallway);
		}
		HallwayTemp.Empty();
	}
	Hallways.Append(Ans);
	Hallways = Ans;
	return Rooms;
}

TArray<FBox2D> FDungeonAlg::BuildHallways(TArray<FBox2D> Rooms, FBox2D Hallway)
{
	TArray<FBox2D> Ans;

	if (Hallway.GetSize().X==HallwayWidth)
	{
		Rooms.Sort([](const FBox2D& Room1, const FBox2D& Room2)
		{
			return Room1.Min.Y > Room2.Min.Y;
		});
		TArray<FEdge> Borders;
		Ans.Add(FBox2D(Hallway.Min, FVector2D(Hallway.Max.X, Rooms[0].Min.Y)));
		Borders.Add(FEdge( FVector(Hallway.Min.X, Rooms[0].Min.Y, 100.0f), FVector(Hallway.Max.X, Rooms[0].Min.Y, 100.0f)));
		for (int32 i = 0; i < Rooms.Num() - 1; ++i)
		{
			Ans.Add(FBox2D(FVector2D(Hallway.Min.X, Rooms[i].Max.Y), FVector2D(Hallway.Max.X, Rooms[i + 1].Min.Y)));
			
			Borders.Add(FEdge( FVector(Hallway.Min.X, Rooms[i].Max.Y, 100.0f), FVector(Hallway.Max.X, Rooms[i].Max.Y, 100.0f)));
			//Borders.Swap(0, 1);
			RoomsBorders->Add(Borders);
			Borders.Empty();
			Borders.Add(FEdge( FVector(Hallway.Min.X, Rooms[i + 1].Min.Y, 100.0f), FVector(Hallway.Max.X, Rooms[i + 1].Min.Y, 100.0f)));
			
		}
		Ans.Add(FBox2D(FVector2D(Hallway.Min.X, Rooms[Rooms.Num()-1].Max.Y), Hallway.Max));
		Borders.Add(FEdge(FVector(Hallway.Min.X, Rooms[Rooms.Num() - 1].Max.Y, 100.0f), FVector(Hallway.Max.X, Rooms[Rooms.Num() - 1].Max.Y, 100.0f)));
		//Borders.Swap(0, 1);
		RoomsBorders->Add(Borders);
	}
	else if (Hallway.GetSize().Y == HallwayWidth)
	{
		Rooms.Sort([](const FBox2D& Room1, const FBox2D& Room2)
		{
			return Room1.Min.X < Room2.Min.X;
		});
		TArray<FEdge> Borders;
		Ans.Add(FBox2D(Hallway.Min, FVector2D(Rooms[0].Min.X, Hallway.Max.Y)));
		Borders.Add(FEdge(FVector(Rooms[0].Min.X, Hallway.Min.Y, 100.0f), FVector(Rooms[0].Min.X, Hallway.Max.Y, 100.0f)));
		for (int32 i = 0; i < Rooms.Num() - 1; ++i)
		{
			Ans.Add(FBox2D(FVector2D(Rooms[i].Max.X, Hallway.Min.Y), FVector2D(Rooms[i+1].Min.X, Hallway.Max.Y)));
			Borders.Add(FEdge(FVector(Rooms[i].Max.X, Hallway.Min.Y, 100.0f), FVector(Rooms[i].Max.X, Hallway.Max.Y, 100.0f)));
			RoomsBorders->Add(Borders);
			Borders.Empty();
			Borders.Add(FEdge(FVector(Rooms[i + 1].Min.X, Hallway.Min.Y, 100.0f), FVector(Rooms[i + 1].Min.X, Hallway.Max.Y, 100.0f)));
		}
		Ans.Add(FBox2D(FVector2D(Rooms[Rooms.Num() - 1].Max.X, Hallway.Min.Y), Hallway.Max));
		Borders.Add(FEdge(FVector(Rooms[Rooms.Num() - 1].Max.X, Hallway.Min.Y, 100.0f), FVector(Rooms[Rooms.Num() - 1].Max.X, Hallway.Max.Y, 100.0f)));
		RoomsBorders->Add(Borders);
	}
	
	return Ans;
}




FDungeonAlg::~FDungeonAlg()
{
	delete Thread;
	Thread = nullptr;
}

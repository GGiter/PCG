// Fill out your copyright notice in the Description page of Project Settings.

#include "Delaunay.h"



Delaunay::Delaunay()
{
}

Delaunay::~Delaunay()
{
}

TArray<FTriangle> Delaunay::Triangulate(TArray<FVector>& NewVertices)
{
	Vertices = NewVertices;

	float MinX = NewVertices[0].X;
	float MinY = NewVertices[0].Y;
	float MinZ = NewVertices[0].Z;
	float MaxX = MinX, MaxY = MinY, MaxZ = MinZ;
	for (FVector Vertice : Vertices)
	{
		if (Vertice.X < MinX)
		{
			MinX = Vertice.X;
		}
		if (Vertice.Y < MinY)
		{
			MinY = Vertice.Y;
		}
		if (Vertice.Z < MinZ)
		{
			MinZ = Vertice.Z;
		}
		if (Vertice.X > MaxX)
		{
			MaxX = Vertice.X;
		}
		if (Vertice.Y > MaxY)
		{
			MaxY = Vertice.Y;
		}
		if (Vertice.Z > MaxZ)
		{
			MaxZ = Vertice.Z;
		}
	}

	float dX = MaxX - MinX;
	float dY = MaxY - MinY;
	float dZ = MaxZ - MinZ;
	float DeltaMax = FMath::Max3(dX, dY, dZ);
	float MidX = (MinX + MaxX) / 2.0f;
	float MidY = (MinY + MaxY) / 2.0f;
	float MidZ = (MinZ + MaxZ) / 2.0f;

	FVector P1(MidX - 20.0f * DeltaMax, MidY - DeltaMax, MidZ - DeltaMax);
	FVector P2(MidX, MidY + 20.0f * DeltaMax, MidZ + 20.0f * DeltaMax);
	FVector P3(MidX + 20.0f * DeltaMax, MidY - DeltaMax, MidZ - DeltaMax);

	Triangles.Add(FTriangle(P1, P2, P3));
	for (FVector Vertice : Vertices)
	{
		TArray<FEdgeD> Polygon;
		
			for (FTriangle Triangle : Triangles)
			{
				
				if (Triangle.CircumSphereContains(Vertice))
				{
					Triangle.isBad = true;
					Polygon.Add(Triangle.E1);
					Polygon.Add(Triangle.E2);
					Polygon.Add(Triangle.E3);
				}
				
			}
			for (FTriangle Triangle : Triangles)
			{
				if (Triangle.isBad)
				{
					Triangles.Remove(Triangle);
				}
			}
		
		
		//UE_LOG(LogTemp, Warning, TEXT("%d"), Triangles.Num());
		if (Polygon.Num() > 0)
		{
			for (FEdgeD E1 : Polygon)
			{
				for (FEdgeD E2 : Polygon)
				{
					if (Polygon.Find(E1) == Polygon.Find(E2))
					{
						continue;
					}
					else if (E1 == E2)
					{
						E1.isBad = true;
						E2.isBad = true;
					}

				}
			}
			for (FEdgeD Edge : Polygon)
			{
				if (Edge.isBad)
				{
					Polygon.Remove(Edge);
				}
				else
				{
					Triangles.Add(FTriangle(Edge.P1, Edge.P2, Vertice));
				}
			}
		}
		

	

	}

	for (FTriangle Triangle : Triangles)
	{
		if (Triangle.ContainsVertex(P1) || Triangle.ContainsVertex(P2) || Triangle.ContainsVertex(P3))
		{
			Triangles.Remove(Triangle);
		}
		else
		{
			Edges.Add(Triangle.E1);
			Edges.Add(Triangle.E2);
			Edges.Add(Triangle.E3);
		}
	}

	return Triangles;
}

const TArray<FTriangle>& Delaunay::GetTriangles() const
{
	return Triangles;
}

const TArray<FEdgeD>& Delaunay::GetEdges() const
{
	return Edges;
}

const TArray<FVector>& Delaunay::GetVertices() const
{
	return Vertices;
}

void FTriangle::CalculateSphere()
{
	FVector AC = P3 - P1;
	FVector AB = P2 - P1;
	FVector ABXAC = FVector::CrossProduct(AB, AC);

	FVector ToCircumSphereCenter = (FVector::CrossProduct(ABXAC, AB)*AC.Size() + FVector::CrossProduct(AC, ABXAC)*AB.Size()) / (2.0f*ABXAC.Size());
	CircumSphereRadius = ToCircumSphereCenter.Size();
	CSS = P1 + ToCircumSphereCenter;
}

bool FTriangle::CircumSphereContains(const FVector & V)
{
	float Dist = FMath::Square(V.X - CSS.X) + FMath::Square(V.Y - CSS.Y) + FMath::Square(V.Z - CSS.Z);
	if (Dist < FMath::Square(CircumSphereRadius))
		return true;
	else
		return false;
}

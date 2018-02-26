// Fill out your copyright notice in the Description page of Project Settings.

#include "Delaunay.h"



Delaunay::Delaunay()
{
}

Delaunay::~Delaunay()
{
}

TArray<FTriangle> Delaunay::Triangulate(TArray<FVector2D>& NewVertices)
{
	Vertices = NewVertices;
	Triangles.Empty();
	Edges.Empty();
	float MinX = NewVertices[0].X;
	float MinY = NewVertices[0].Y;
	float MaxX = MinX, MaxY = MinY;
	for (FVector2D Vertice : NewVertices)
	{
		if (Vertice.X < MinX)
		{
			MinX = Vertice.X;
		}
		if (Vertice.Y < MinY)
		{
			MinY = Vertice.Y;
		}
		if (Vertice.X > MaxX)
		{
			MaxX = Vertice.X;
		}
		if (Vertice.Y > MaxY)
		{
			MaxY = Vertice.Y;
		}
	}

	float dX = MaxX - MinX;
	float dY = MaxY - MinY;
	float DeltaMax = FMath::Max(dX, dY);
	float MidX = (MinX + MaxX) / 2.0f;
	float MidY = (MinY + MaxY) / 2.0f;

	FVector2D P1(MidX - 20.0f * DeltaMax, MidY - DeltaMax);
	FVector2D P2(MidX, MidY + 20.0f * DeltaMax);
	FVector2D P3(MidX + 20.0f * DeltaMax, MidY - DeltaMax);

	Triangles.Add(FTriangle(P1, P2, P3));
	for (FVector2D Vertice : NewVertices)
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
			Triangles.RemoveAll([](const FTriangle Triangle)
			{
				return Triangle.isBad;
			});
		
			int32 ii = 0, jj = 0;
		//UE_LOG(LogTemp, Warning, TEXT("%d"), Triangles.Num());
			for (FEdgeD E1 : Polygon)
			{
				ii++;
				for (FEdgeD E2 : Polygon)
				{
					jj++;
					if (ii==jj)
					{
						continue;
					}
					else if (E1 == E2)
					{
						E1.isBad = true;
						E2.isBad = true;
					}

				}
				jj = 0;
			}
			Polygon.RemoveAll([](const FEdgeD EdgeD)
			{
				return EdgeD.isBad;
			});
			for (FEdgeD Edge : Polygon)
			{
					Triangles.Add(FTriangle(Edge.P1, Edge.P2, Vertice));	
			}
		
	

	}
	Triangles.RemoveAll([P1,P2,P3](FTriangle Triangle)
	{
		return Triangle.ContainsVertex(P1) || Triangle.ContainsVertex(P2) || Triangle.ContainsVertex(P3);
	});
	for (FTriangle Triangle : Triangles)
	{	
			Edges.Add(Triangle.E1);
			Edges.Add(Triangle.E2);
			Edges.Add(Triangle.E3);
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

const TArray<FVector2D>& Delaunay::GetVertices() const
{
	return Vertices;
}


bool FTriangle::CircumSphereContains(const FVector2D & V)
{
	float ab = (P1.X * P1.X) + (P1.Y * P1.Y);
	float cd = (P2.X * P2.X) + (P2.Y * P2.Y);
	float ef = (P3.X * P3.X) + (P3.Y * P3.Y);

	float circum_x = (ab * (P3.Y - P2.Y) + cd * (P1.Y - P3.Y) + ef * (P2.Y - P1.Y)) / (P1.X * (P3.Y - P2.Y) + P2.X * (P1.Y - P3.Y) + P3.X * (P2.Y - P1.Y)) / 2.f;
	float circum_y = (ab * (P3.X - P2.X) + cd * (P1.X - P3.X) + ef * (P2.X - P1.X)) / (P1.Y * (P3.X - P2.X) + P2.Y * (P1.X - P3.X) + P3.Y * (P2.X - P1.X)) / 2.f;
	float circum_radius = FMath::Sqrt(((P1.X - circum_x) * (P1.X - circum_x)) + ((P1.Y - circum_y) * (P1.Y - circum_y)));

	float dist = FMath::Sqrt(((V.X - circum_x) * (V.X - circum_x)) + ((V.Y - circum_y) * (V.Y - circum_y)));
	return dist <= circum_radius;
}

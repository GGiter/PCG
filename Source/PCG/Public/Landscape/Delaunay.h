// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"




class FEdgeD
{
public:

	FEdgeD() {}
	FEdgeD(const FVector &_P1, const FVector &_P2) : P1(_P1), P2(_P2), isBad(false) {}
	FEdgeD(const FEdgeD &E) : P1(E.P1), P2(E.P2), isBad(false) {}
	FVector P1, P2;
	bool isBad;
	
	
};

inline bool operator == (const FEdgeD &E1, const FEdgeD &E2)
{
	return (E1.P1 == E2.P1&&E1.P2 == E2.P2) || (E1.P1 == E2.P2&&E1.P2 == E2.P1);
}


class FTriangle
{

public:

	FTriangle() { CalculateSphere(); }
	FTriangle(const FVector &_P1, const FVector &_P2, const FVector &_P3)
		: P1(_P1),P2(_P2),P3(_P3),
		  E1(_P1,_P2), E2(_P2,_P3), E3(_P3,_P1), isBad(false)
	{
		CalculateSphere();
	}
	FVector P1, P2, P3;
	FVector CSS;
	float CircumSphereRadius;
	FEdgeD E1, E2, E3;
	bool isBad;
	void CalculateSphere();
	bool ContainsVertex(const FVector &V)
	{
		return P1 == V || P2 == V || P3 == V;
	}
	bool CircumSphereContains(const FVector &V);

	
};
inline bool operator == (const FTriangle& T1, const FTriangle& T2)
{
	return T1.P1 == T2.P1 && T1.P2 == T2.P2 && T1.P3 == T2.P3;
}
class Delaunay
{
public:
	Delaunay();
	~Delaunay();

	TArray<FTriangle> Triangulate(TArray<FVector> &NewVertices);
	const TArray<FTriangle>& GetTriangles() const;
	const TArray<FEdgeD>& GetEdges() const;
	const TArray<FVector>& GetVertices() const;
private:
	TArray<FTriangle> Triangles;
	TArray<FEdgeD> Edges;
	TArray<FVector> Vertices;
};

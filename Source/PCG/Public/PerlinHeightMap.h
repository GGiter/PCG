// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"

/**
 * 
 */
//Calculates PerlinMap between two Points
class PerlinHeightMap
{
public:
	PerlinHeightMap(FVector NewStart, FVector NewEnd, float NewHeight, int32 NewRows, int32 NewColums,float NewBlockWidth)
		: Start(NewStart), End(NewEnd), Height(NewHeight), Rows(NewRows), Columns(NewColums),BlockWidth(NewBlockWidth)
	{
		GenerateHeightMap();
	}
	~PerlinHeightMap();
	TArray<TArray<FVector> > GetHeightMap() const;
private:
	void GenerateHeightMap();
	FVector Start;
	FVector End;
	float Height, BlockWidth;
	int32 Rows;
	int32 Columns;
	TArray<TArray<FVector> > HeightMap;
};

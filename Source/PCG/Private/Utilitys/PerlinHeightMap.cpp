// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinHeightMap.h"
#include "PerlinNoise.h"


TArray<TArray<FVector>> PerlinHeightMap::GetHeightMap() const
{
	return HeightMap;
}

void PerlinHeightMap::GenerateHeightMap()
{
	PerlinNoise PN;
	float yoff = -0.1f;
	int32 y, x;
	for (y = 0; y < Rows; ++y)
	{
		float xoff = 0.0f;
		TArray<FVector> Temp;
		for (x = 0; x < Columns; ++x)
		{

			float ZValue = (FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(-Height, Height), PN.Noise(xoff, yoff, 0.0f)));
			xoff += 0.2f;
			FVector Location(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, Columns-1), FVector2D(Start.X, End.X), y), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, Rows), FVector2D(Start.Y, End.Y), x), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, Rows-1), FVector2D(Start.Z, End.Z), y));
			Temp.Add(Location + FVector(y*BlockWidth, x*BlockWidth, ZValue));
		}
		HeightMap.Add(Temp);
		yoff += 0.2f;
	}
}






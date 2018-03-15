// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinMap.h"
#include "Public/Utilitys/PerlinNoise.h"


// Sets default values
APerlinMap::APerlinMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>("RuntimeMesh");

}
void APerlinMap::Generate()
{
	PointList.Empty();
	Terrain.Empty();
	GenerateHeightMap();
	RuntimeMesh->ClearAllMeshSections();
	RenderLandscape();
}
void APerlinMap::SetRows(int32  NewRows)
{
	Rows = NewRows;
}
void APerlinMap::SetColumns(int32  NewColumns)
{
	Columns = NewColumns;
}
void APerlinMap::SetBlockWidth(float  NewBlockWidth)
{
	BlockWidth = NewBlockWidth;
}
int32 APerlinMap::GetRows() const
{
	return Rows;
}
int32 APerlinMap::GetColumns() const
{
	return Columns;
}
float APerlinMap::GetBlockWidth() const
{
	return BlockWidth;
}
FName APerlinMap::GetLandscapeName() const
{
	return LandscapeName;
}
void APerlinMap::GenerateHeightMap()
{
	PerlinNoise PN;
	float yoff = -0.1f;
	int32 y, x;
	for (y = 0; y < Rows; ++y)
	{
		float xoff = 0.0f;
		for (x = 0; x < Columns; ++x)
		{
			
			Terrain.Add(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(-100.0f, 100.0f), PN.Noise(xoff, yoff,0.0f)));
			xoff += 0.2f;
			PointList.Add(GetActorLocation() + FVector(y*BlockWidth, x*BlockWidth, Terrain[Terrain.Num() - 1]));
		}
		yoff += 0.2f;
		
	}
	
}

void APerlinMap::RenderLandscape()
{
	TArray<FVector> Vertices = PointList;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;
	int32 y, x;
	for (y = 1; y < Rows; ++y)
	{
		for (x = 0; x < Columns-1; ++x)
		{
			int32 P3 = y*Columns + x, P0 = (y - 1)*Columns + x, P1 = (y - 1)*Columns + x + 1, P2 = y*Columns + x + 1;
			Normals.Add(FVector(0, 0, 1));
			Normals.Add(FVector(0, 0, 1));
			Normals.Add(FVector(0, 0, 1));
			Normals.Add(FVector(0, 0, 1));
			Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
			Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
			Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
			Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
			VertexColors.Add(FColor::White);
			TextureCoordinates.Add(FVector2D(0, 0));
			VertexColors.Add(FColor::White);
			TextureCoordinates.Add(FVector2D(0, 1));
			VertexColors.Add(FColor::White);
			TextureCoordinates.Add(FVector2D(1, 1));
			VertexColors.Add(FColor::White);
			TextureCoordinates.Add(FVector2D(1, 0));
			Triangles.Add(P0);
			Triangles.Add(P1);
			Triangles.Add(P2);
			Triangles.Add(P0);
			Triangles.Add(P2);
			Triangles.Add(P3);
		}
	}
	RuntimeMesh->CreateMeshSection(0, Vertices, Triangles, Normals, TextureCoordinates, VertexColors, Tangents);
}

// Called when the game starts or when spawned
void APerlinMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerlinMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


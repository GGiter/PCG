// Fill out your copyright notice in the Description page of Project Settings.

#include "DSMapGenerator.h"
#include "Delaunay.h"

// Sets default values
ADSMapGenerator::ADSMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>("RuntimeMesh");
	
}

// Called when the game starts or when spawned
void ADSMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	Generate();
}





void ADSMapGenerator::DrawMesh()
{
	
	float x, y;
	for (y = 0; y < WorldSize; ++y)
	{
		for (x = 0; x < WorldSize; ++x)
		{
			PointList.Add(FVector(x, y, HeightMap[TPair<float, float>(x, y)]));
			
		}
	}
	TArray<FVector> Vertices = PointList;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;
	for (y = 1; y < WorldSize; ++y)
	{
		for (x = 0; x < WorldSize-1; ++x)
		{
			int32 P3 = y*WorldSize + x, P0 = (y - 1)*WorldSize + x, P1 = (y - 1)*WorldSize + x + 1, P2 = y*WorldSize + x + 1;
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

void ADSMapGenerator::Divide(float Size)
{
	float x, y, Half = Size / 2;
	float Scale = Roughness*Size;

	if (Half < 1) return;

	for (y = Half; y < Max; y += Size)
	{
		for (x = Half; x < Max; x += Size)
		{
			Square(x, y, Half, FMath::FRand()*Scale * 2 - Scale);
		}
	}
	for (y = 0; y <= Max; y += Half)
	{
		for (x = FMath::Fmod(y + Half,Size); x <= Max; x += Size)
		{
			Diamond(x, y, Half, FMath::FRand()*Scale * 2 - Scale);
		}
	}
	Divide(Size / 2);
}

void ADSMapGenerator::Diamond(float x, float y, float Size, float Offset)
{
	
	if (HeightMap.Find(TPair<float, float>(x, y - Size)) == nullptr)
		HeightMap.Add(TPair<float, float>(x, y - Size), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x + Size, y)) == nullptr)
		HeightMap.Add(TPair<float, float>(x + Size, y), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x, y + Size)) == nullptr)
		HeightMap.Add(TPair<float, float>(x, y + Size), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x - Size, y)) == nullptr)
		HeightMap.Add(TPair<float, float>(x - Size, y), 0.0f);

	float AVG = (HeightMap[TPair<float, float>(x, y - Size)] + HeightMap[TPair<float, float>(x+Size, y)] + HeightMap[TPair<float, float>(x, y + Size)] + HeightMap[TPair<float, float>(x-Size, y)]) / 4.0f;
	HeightMap.Add(TPair<float, float>(x, y), AVG + Offset);
}

void ADSMapGenerator::Square(float x, float y, float Size, float Offset)
{
	if (HeightMap.Find(TPair<float, float>(x, y - Size)) == nullptr)
		HeightMap.Add(TPair<float, float>(x, y - Size), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x + Size, y)) == nullptr)
		HeightMap.Add(TPair<float, float>(x + Size, y), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x, y + Size)) == nullptr)
		HeightMap.Add(TPair<float, float>(x, y + Size), 0.0f);
	if (HeightMap.Find(TPair<float, float>(x - Size, y)) == nullptr)
		HeightMap.Add(TPair<float, float>(x - Size, y), 0.0f);

	float AVG = (HeightMap[TPair<float, float>(x-Size, y - Size)] + HeightMap[TPair<float, float>(x + Size, y-Size)] + HeightMap[TPair<float, float>(x+Size, y + Size)] + HeightMap[TPair<float, float>(x, y+Size)]) / 4.0f;
	HeightMap.Add(TPair<float, float>(x, y), AVG + Offset);
}

// Called every frame
void ADSMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSMapGenerator::Generate()
{
	RuntimeMesh->ClearAllMeshSections();
	HeightMap.Empty();
	PointList.Empty();
	WorldSize = FMath::Pow(2, Detail) + 1.0f;
	Max = WorldSize - 1.0f;
	HeightMap.Add(TPair<float, float>(0.0f, 0.0f), Max / 2.0f);
	HeightMap.Add(TPair<float, float>(Max, 0.0f), Max / 2.0f);
	HeightMap.Add(TPair<float, float>(Max, Max), Max / 2.0f);
	HeightMap.Add(TPair<float, float>(0.0f, Max), Max / 2.0f);
	Divide(Max);
	DrawMesh();
}

void ADSMapGenerator::SetLandscapeScale()
{
	RuntimeMesh->SetRelativeScale3D(LandscapeScale);
}

void ADSMapGenerator::SetDetail(float  NewDetail)
{
	Detail = NewDetail;
}

void ADSMapGenerator::SetRoughness(float  NewRoughness)
{
	Roughness = NewRoughness;
}

float ADSMapGenerator::GetDetail() const
{
	return Detail;
}

float ADSMapGenerator::GetRoughness() const
{
	return Roughness;
}

FName ADSMapGenerator::GetLandscapeName() const
{
	return LandscapeName;
}


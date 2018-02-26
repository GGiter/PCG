// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonActor.h"


// Sets default values
ADungeonActor::ADungeonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>("RuntimeMesh");
}

// Called when the game starts or when spawned
void ADungeonActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FDungeonAlg::IsThreadFinished() && bC == false)
	{
		bC = true;
		UE_LOG(LogTemp, Warning, TEXT("PointList Size %d"), PointLists.Num());
		if(PointLists.Num()>0)
		Render();
		
	}
}
void ADungeonActor::Render()
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;
	int32 index2 = 0;
	for (TArray<FVector> PointList : PointLists)
	{
		if (PointList.Num() == 4)
		{
			Vertices.Append(PointList);
			index2 += 1;
		}
		
	}
	NumOfSections = 0;
	for (TArray<FVector> PointList : PointLists)
	{

		if (PointList.Num() == 4)
		{
			TArray<FVector> Temp, Temp2;
			Temp2.Append(PointList);
			for (FVector Point : PointList)
			{
				Temp.Add(FVector(Point.X, Point.Y, 100.0f));
			}
			Vertices.Append(Temp);
			Temp2.Append(Temp);
			GenerateGround(Temp2, NumOfSections);
			NumOfSections++;
		}
			
	}
	for (TArray<FVector> PointList : PointLists)
	{

		if (PointList.Num() == 4&&PointLists.Find(PointList)>=NumOfTypes[2][0])
		{
			TArray<FVector> Temp, Temp2;
			
			for (FVector Point : PointList)
			{
				Temp.Add(FVector(Point.X, Point.Y, 100.0f));
			}
			Temp2.Append(Temp);
			Temp.Empty();
			for (FVector Point : PointList)
			{
				Temp.Add(FVector(Point.X, Point.Y, 500.0f));
			}
			Vertices.Append(Temp);
			Temp2.Append(Temp);
			RenderHallways(Temp2, NumOfSections);
			NumOfSections++;
		}

	}
	int32 index = 0;
	UE_LOG(LogTemp, Warning, TEXT("DungeonAlg is Done %d"),Vertices.Num());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Vertices[0].ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Vertices[1].ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Vertices[2].ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Vertices[3].ToString());
	for (TArray<FVector> PointList : PointLists)
	{
		
		if (PointList.Num() == 4)
		{
			int32 P3 = 3+index*4, P0 = index * 4, P1 = 1 + index * 4, P2 = 2 + index * 4;
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
			int32 P7 = 3 + index2 * 4, P4 = index2 * 4, P5 = 1 + index2 * 4, P6 = 2 + index2 * 4;
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
			Triangles.Add(P4);
			Triangles.Add(P5);
			Triangles.Add(P6);
			Triangles.Add(P4);
			Triangles.Add(P6);
			Triangles.Add(P7);

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
			Triangles.Add(P3);
			Triangles.Add(P4);
			Triangles.Add(P0);
			Triangles.Add(P3);
			Triangles.Add(P7);
			Triangles.Add(P4);

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
			Triangles.Add(P2);
			Triangles.Add(P5);
			Triangles.Add(P1);
			Triangles.Add(P2);
			Triangles.Add(P6);
			Triangles.Add(P5);

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
			Triangles.Add(P2);
			Triangles.Add(P3);
			Triangles.Add(P7);
			Triangles.Add(P2);
			Triangles.Add(P6);
			Triangles.Add(P7);

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
			Triangles.Add(P1);
			Triangles.Add(P0);
			Triangles.Add(P4);
			Triangles.Add(P1);
			Triangles.Add(P5);
			Triangles.Add(P4);
			index++;
			index2++;
		}
		
	}
	SetColors();
}
void ADungeonActor::GenerateGround(TArray<FVector> Vertices, int32 SectionIndex)
{
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;
	TArray<FVector> Temp = Vertices;
	Vertices.Empty();
	//UpSide
	for (int i = 7; i >= 4; --i)
	{
		Vertices.Add(Temp[i]);
	}
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	//LeftSide
	Vertices.Add(Temp[0]);
	Vertices.Add(Temp[4]);
	Vertices.Add(Temp[7]);
	Vertices.Add(Temp[3]);
	Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));

	//BackSide
	Vertices.Add(Temp[1]);
	Vertices.Add(Temp[5]);
	Vertices.Add(Temp[4]);
	Vertices.Add(Temp[0]);
	Normals.Add(FVector(0.0f, 1.0f, 0.0f));
	Normals.Add(FVector(0.0f, 1.0f, 0.0f));
	Normals.Add(FVector(0.0f, 1.0f, 0.0f));
	Normals.Add(FVector(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));

	//RightSide
	Vertices.Add(Temp[2]);
	Vertices.Add(Temp[6]);
	Vertices.Add(Temp[5]);
	Vertices.Add(Temp[1]);
	Normals.Add(FVector(1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(1.0f, 0.0f, 0.0f));
	Normals.Add(FVector(1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));

	//FaceSide
	Vertices.Add(Temp[3]);
	Vertices.Add(Temp[7]);
	Vertices.Add(Temp[6]);
	Vertices.Add(Temp[2]);
	Normals.Add(FVector(0.0f, -1.0f, 0.0f));
	Normals.Add(FVector(0.0f, -1.0f, 0.0f));
	Normals.Add(FVector(0.0f, -1.0f, 0.0f));
	Normals.Add(FVector(0.0f, -1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));

	//BottomSide
	Vertices.Add(Temp[3]);
	Vertices.Add(Temp[2]);
	Vertices.Add(Temp[1]);
	Vertices.Add(Temp[0]);
	Normals.Add(FVector(0.0f, 0.0f, -1.0f));
	Normals.Add(FVector(0.0f, 0.0f, -1.0f));
	Normals.Add(FVector(0.0f, 0.0f, -1.0f));
	Normals.Add(FVector(0.0f, 0.0f, -1.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	

	for (int32 i = 0; i < 6; ++i)
	{
		Triangles.Add(i*4);
		Triangles.Add(i * 4 +1);
		Triangles.Add(i * 4 +3);
		Triangles.Add(i * 4 +1);
		Triangles.Add(i * 4 +2);
		Triangles.Add(i * 4 +3);
		
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(0, 0));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(0, 1));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(1, 1));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(1, 0));
	}
	RuntimeMesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, TextureCoordinates, VertexColors, Tangents);
	
}
void ADungeonActor::RenderHallways(TArray<FVector> NewVertices, int32 SectionIndex)
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;
	if (FVector::Dist(NewVertices[0],NewVertices[3])>FVector::Dist(NewVertices[0], NewVertices[1])) //vertical
	{
		//RightSide
		Vertices.Add(NewVertices[2]);
		Vertices.Add(NewVertices[6]);
		Vertices.Add(NewVertices[5]);
		Vertices.Add(NewVertices[1]);
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Vertices.Add(NewVertices[2]);
		Vertices.Add(NewVertices[6]);
		Vertices.Add(NewVertices[5]);
		Vertices.Add(NewVertices[1]);
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));

		//LeftSide
		Vertices.Add(NewVertices[0]);
		Vertices.Add(NewVertices[4]);
		Vertices.Add(NewVertices[7]);
		Vertices.Add(NewVertices[3]);
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, -1.0f, 0.0f));
		Vertices.Add(NewVertices[0]);
		Vertices.Add(NewVertices[4]);
		Vertices.Add(NewVertices[7]);
		Vertices.Add(NewVertices[3]);
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Normals.Add(FVector(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(0.0f, 1.0f, 0.0f));
	}
	else //horizontal
	{
		//Right
		Vertices.Add(NewVertices[3]);
		Vertices.Add(NewVertices[7]);
		Vertices.Add(NewVertices[6]);
		Vertices.Add(NewVertices[2]);
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Vertices.Add(NewVertices[3]);
		Vertices.Add(NewVertices[7]);
		Vertices.Add(NewVertices[6]);
		Vertices.Add(NewVertices[2]);
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));

		//Left
		Vertices.Add(NewVertices[1]);
		Vertices.Add(NewVertices[5]);
		Vertices.Add(NewVertices[4]);
		Vertices.Add(NewVertices[0]);
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Normals.Add(FVector(0.0f, 1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(-1.0f, 0.0f, 0.0f));
		Vertices.Add(NewVertices[1]);
		Vertices.Add(NewVertices[5]);
		Vertices.Add(NewVertices[4]);
		Vertices.Add(NewVertices[0]);
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Normals.Add(FVector(0.0f, -1.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
		Tangents.Add(FRuntimeMeshTangent(1.0f, 0.0f, 0.0f));
	}
	for (int32 i = 0; i < 2; ++i)
	{
		Triangles.Add(i * 4);
		Triangles.Add(i * 4 + 1);
		Triangles.Add(i * 4 + 3);
		Triangles.Add(i * 4 + 1);
		Triangles.Add(i * 4 + 2);
		Triangles.Add(i * 4 + 3);

		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(0, 0));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(0, 1));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(1, 1));
		VertexColors.Add(FColor::White);
		TextureCoordinates.Add(FVector2D(1, 0));
	}
	RuntimeMesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, TextureCoordinates, VertexColors, Tangents);
}
void ADungeonActor::Generate()
{
	FDungeonAlg::JoyInit(PointLists,100,300.0f,NumOfTypes);
}


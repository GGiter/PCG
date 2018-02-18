// Fill out your copyright notice in the Description page of Project Settings.

#include "MengerSponge.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

AMengerSponge::AMengerSponge()
{
	PrimaryActorTick.bCanEverTick = true;
	Position = { 0.0f,0.0f,0.0f };
	Transforms.Add(FTransform(FRotator(0, 0, 0), GetActorLocation(), FVector(1.0f, 1.0f, 1.0f)));
	Iterations = 0;
	NumOfIterations = 2;
}
void AMengerSponge::GenerateTransforms(FTransform BoxTransform, TArray<FTransform>& TransformArray)
{
	for (int32 x = -1; x < 2; ++x)
	{
		for (int32 y = -1; y < 2; ++y)
		{
			for (int32 z = -1; z < 2; ++z)
			{
				int32 sum = FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z);
				if (sum > 1) {
					UE_LOG(LogTemp, Warning, TEXT("Size"));
					FVector Location = { x*r,y*r,z*r };
					FRotator Rotation = { 0.0f,0.0f,0.0f };
					FVector Scale = { SizeR,SizeR,SizeR };
					TransformArray.Add(FTransform(Rotation, BoxTransform.GetLocation() + Location, Scale));
				}

			}
		}
	}
}
void AMengerSponge::SpawnBoxes()
{
	for (FTransform Transform : Transforms)
	{
		HISMActor->AddInstance(Transform);
	}
}
void AMengerSponge::BeginPlay()
{
	Super::BeginPlay();
	Generate();
}
void AMengerSponge::Generate()
{
	r = 256.0f;
	SizeR = 1.0f;
	for (int32 i = 0; i < NumOfIterations; ++i)
	{
		r = r / 3;
		SizeR = r / 3;
		TArray<FTransform> Next;
		for (FTransform Transform : Transforms)
		{
			GenerateTransforms(Transform, Next);
		}
		Transforms = Next;
	}
	SpawnBoxes();
	//UE_LOG(LogTemp, Warning, TEXT("NUMBER %d"), HISMActor->GetInstanceCount());
}





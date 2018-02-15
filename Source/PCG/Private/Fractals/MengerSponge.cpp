// Fill out your copyright notice in the Description page of Project Settings.

#include "MengerSponge.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"

AMengerSponge::AMengerSponge()
{
	PrimaryActorTick.bCanEverTick = true;
	Position = { 0.0f,0.0f,0.0f };
	r = Size.X;
	Iterations = 0;
}
void AMengerSponge::BeginPlay()
{
	Super::BeginPlay();
	HISMActor->AddInstance(FTransform(FRotator{ 0.0f,0.0f,0.0f }, FVector{ 0.0f,0.0f,0.0f }, FVector{ r,r,r }));
}
void AMengerSponge::Generate()
{
	Super::Generate();
	Iterations++;
		for (int32 j = 0; j < HISMActor->GetInstanceCount(); ++j)
		{
			
			GenerateBoxes();
		}
	
		
}
void AMengerSponge::GenerateBoxes()
{
	r = 256.0f;
	
	if (Iterations == 1)
	{
		
		HISMActor->RemoveInstance(0);
	}
	for (int32 i = 0; i < (Iterations - 1) * 27; ++i)
	{
		HISMActor->RemoveInstance(i);
	}
	
	for (int32 x = -1; x < 2; ++x)
	{
		for (int32 y = -1; y < 2; ++y)
		{
			for (int32 z = -1; z < 2; ++z)
			{
				int32 sum = FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z);
				if (sum > 1) {
					UE_LOG(LogTemp, Warning, TEXT("Size"));
					float newR = r / 3;
					FVector Location = { Position.X + x*newR,Position.Y + y*newR,Position.Z + z*newR };
					FRotator Rotation = { 0.0f,0.0f,0.0f };
					FVector Scale = { newR,newR,newR };
					HISMActor->AddInstance(FTransform(Rotation, Location, Scale));
					
				}
				
			}
		}
	}
	
}




// Fill out your copyright notice in the Description page of Project Settings.

#include "FractalActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Public/Fractals/FractalActor.h"

// Sets default values
AFractalActor::AFractalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HISMActor = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("HISMActor");
	DrawComponent = CreateDefaultSubobject<UFractalDrawComponent>("DrawComponent");
	DrawComponent->FractalActorRef = this;
	Size = { 1,1,1 };
}
// Called when the game starts or when spawned
void AFractalActor::BeginPlay()
{
	Super::BeginPlay();
	Generate();
}

// Called every frame
void AFractalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AFractalActor::Generate()
{
	DestroyBeams();

}
void AFractalActor::SetFractalSize(FVector NewSize)
{
	Size = NewSize;
}

TArray<TPair<FVector, FVector>>  AFractalActor::GetPDIIntstuctions()
{
	return PDIInstructions;
}

FVector AFractalActor::GetCenterPoint()
{
	FVector CenterPoint = { 0.0f,0.0f,100.f };
	for (UParticleSystemComponent* Beam : Beams)
	{
		FVector Temp;
		Beam->GetBeamSourcePoint(0, 0, Temp);
		if(Temp.X > CenterPoint.X)
		{
			CenterPoint.X = Temp.X;
		}
		if (Temp.Y > CenterPoint.Y)
		{
			CenterPoint.Y = Temp.Y;
		}
		if (Temp.Z > CenterPoint.Z)
		{
			CenterPoint.Z = Temp.Z;
		}
		
	}
	CenterPoint /= 2.0f;
	CenterPoint.Z += CenterPoint.X*2.0f;
	
	return CenterPoint;
}

void AFractalActor::DestroyBeams()
{
	for (UParticleSystemComponent* Beam : Beams)
	{
		Beam->SetVisibility(false);
		Beam->DestroyComponent();
	}
	Beams.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Angle isS"));
}

TArray<class UParticleSystemComponent*> AFractalActor::GetBeams()
{
	return Beams;
}

int32 AFractalActor::GetNumOfIterations()
{
	return NumOfIterations;
}

void AFractalActor::SetNumOfIterations(int32 NewNumOfIterations)
{
	NumOfIterations = NewNumOfIterations;
}


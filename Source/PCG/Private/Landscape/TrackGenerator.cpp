// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackGenerator.h"

#include "RuntimeMeshComponent.h"



// Sets default values
ATrackGenerator::ATrackGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	PathSpline->SetupAttachment(RootComponent);


	
}

// Called when the game starts or when spawned
void ATrackGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrackGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FTrackAlg::IsThreadFinished() && bC == false)
	{
		bC = true;
		UE_LOG(LogTemp, Warning, TEXT("TrackPoints %s , %s"), *SurroundingsHM[0][0][0].ToString(), *SurroundingsHM[0][0][1].ToString());
		BuildSpline();
		FTrackAlg::Shutdown();
	}
}

void ATrackGenerator::BuildSpline()
{
	for (FVector Vector : TrackPoints)
	{
		PathSpline->AddSplinePoint(Vector, ESplineCoordinateSpace::Local, false);
	}
	PathSpline->UpdateSpline();
	if (bAutoSpacing)
	{
		Spacing = RoadMesh->GetBounds().BoxExtent.Y;
	}
	
	int32 Division = PathSpline->GetSplineLength() / Spacing;
	for (int32 i = 0; i < Division; ++i)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMesh->SetStaticMesh(RoadMesh);
		FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector StartTangent = PathSpline->GetDirectionAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector EndPos = PathSpline->GetLocationAtDistanceAlongSpline((i+1)*Spacing, ESplineCoordinateSpace::Local);
		FVector EndTangent = PathSpline->GetDirectionAtDistanceAlongSpline((i+1)*Spacing, ESplineCoordinateSpace::Local);
		SplineMesh->SetStartAndEnd(StartPos, StartTangent*TangentScale, EndPos, EndTangent*TangentScale);
		SplineMesh->RegisterComponent();
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		TrackMeshes.Add(SplineMesh);
	}
	//UStaticMesh* TEST=RuntimeMesh->CreateDefaultSubobject<UStaticMesh>(TEXT("TEst"));
}

void ATrackGenerator::Generate()
{
	
	for (USplineMeshComponent* SplineMesh : TrackMeshes)
	{
		SplineMesh->DestroyComponent();
	}
	TrackMeshes.Empty();
	TrackPoints.Empty();
	PathSpline->ClearSplinePoints();
	FTrackAlg::JoyInit(TrackPoints, SurroundingsHM, Ellipse_Width, Ellipse_Height, NumberOfPoints, StartingHeight, Steepness);
	bC = false;
}

void ATrackGenerator::SetSteepness(float NewSteepness)
{
	Steepness = NewSteepness;
}

void ATrackGenerator::SetNewSpacing(float NewSpacing)
{
	Spacing = NewSpacing;
}

void ATrackGenerator::SetNumberOfPoints(int32 NewNumberOfPoints)
{
	NumberOfPoints = NewNumberOfPoints;
}

void ATrackGenerator::SetEllipseWidth(float NewEllipseWidth)
{
	Ellipse_Width = NewEllipseWidth;
}

void ATrackGenerator::SetEllpiseHeight(float NewEllipseHeight)
{
	Ellipse_Height = NewEllipseHeight;
}

float ATrackGenerator::GetSteepness() const
{
	return Steepness;
}

float ATrackGenerator::GetSpacing() const
{
	return Spacing;
}

int32 ATrackGenerator::GetNumberOfPoints() const
{
	return NumberOfPoints;
}

float ATrackGenerator::GetEllipseWidth() const
{
	return Ellipse_Width;;
}

float ATrackGenerator::GetEllipseHeight() const
{
	return Ellipse_Height;
}

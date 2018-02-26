// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonizationTree.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


void AColonizationTree::BeginPlay()
{
	Super::BeginPlay();
	
}

void AColonizationTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FSCTree::IsThreadFinished() && bC==false)
	{
		bC = true;
		SpawnLines();
	}
		
		
}

void AColonizationTree::Test()
{
	FSCTree::JoyInit(Locations);
	//SpawnLines();
}

void AColonizationTree::SpawnLines()
{
	for (TPair<FVector, FVector> Location : Locations)
	{
		UE_LOG(LogTemp, Warning, TEXT("Locations Size -> %s"), *Location.Key.ToString());
		UParticleSystemComponent* ParticleTemp;
		ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FractalPSC, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false);
		ParticleTemp->SetVisibility(true);
		ParticleTemp->SetBeamSourcePoint(0, Location.Key, 0);
		ParticleTemp->SetBeamEndPoint(0, Location.Value);
		Beams.Add(ParticleTemp);
	}
	
}

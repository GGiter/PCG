// Fill out your copyright notice in the Description page of Project Settings.

#include "CantorSet.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACantorSet::ACantorSet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACantorSet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACantorSet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACantorSet::Generate()
{
	
	Super::Generate();
	GenerateRecursive(GetActorLocation().Y, Distance,0);
	
	
}
void ACantorSet::GenerateRecursive(float Start, float End, float Height)
{
	if (NumOfIterations-Height>0)
	{
		UParticleSystemComponent* ParticleTemp;
		ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FractalPSC, GetActorLocation(), GetActorRotation(), false);
		FVector StartBeam{ GetActorLocation().X + -Height * Intermission ,Start ,GetActorLocation().Z};
		FVector EndBeam{ GetActorLocation().X + -Height * Intermission,Start + End ,GetActorLocation().Z};
		ParticleTemp->SetVisibility(true);
		ParticleTemp->SetBeamSourcePoint(0, StartBeam, 0);
		ParticleTemp->SetBeamEndPoint(0, EndBeam);
		Beams.Add(ParticleTemp);
		GenerateRecursive(Start,End/3, Height + 1);
		GenerateRecursive(Start+2 * (End / 3), End/3, Height + 1);
	}
	
}



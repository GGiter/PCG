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
	GenerateRecursive(GetActorLocation().X, Distance,0);
	
	
}
void ACantorSet::GenerateRecursive(float Start, float End, float Height)
{
	if (Size.X-Height>0)
	{
		UParticleSystemComponent* ParticleTemp;
		ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSC, GetActorLocation(), GetActorRotation(), false);
		FVector StartBeam{ Start,GetActorLocation().Y ,GetActorLocation().Z + -Height * Intermission };
		FVector EndBeam{ Start+End,GetActorLocation().Y ,GetActorLocation().Z + -Height * Intermission };
		ParticleTemp->SetVisibility(true);
		ParticleTemp->SetBeamSourcePoint(0, StartBeam, 0);
		ParticleTemp->SetBeamEndPoint(0, EndBeam);
		Beams.Add(ParticleTemp);
		GenerateRecursive(Start,End/3, Height + 1);
		GenerateRecursive(Start+2 * (End / 3), End/3, Height + 1);
	}
	
}



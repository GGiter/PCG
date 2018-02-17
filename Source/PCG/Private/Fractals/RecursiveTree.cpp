// Fill out your copyright notice in the Description page of Project Settings.

#include "RecursiveTree.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARecursiveTree::ARecursiveTree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARecursiveTree::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARecursiveTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARecursiveTree::Generate()
{
	
	Super::Generate();
	GenerateRecursive(GetActorLocation().X, Distance, 0,0,0);


}
void ARecursiveTree::GenerateRecursive(float Start, float End, float Height,float Direction,int32 Lvl)
{
	if (NumOfIterations - Lvl>0)
	{
		UParticleSystemComponent* ParticleTemp;
		ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FractalPSC, GetActorLocation(), GetActorRotation(), false);
		FVector StartBeam;
		if(Lvl>0)
		StartBeam={  Start ,GetActorLocation().Y + Height ,GetActorLocation().Z};
		else
		StartBeam = { Start ,GetActorLocation().Y + Height ,GetActorLocation().Z };
		FVector EndBeam{ Start+End,GetActorLocation().Y + Height + Intermission*Direction ,GetActorLocation().Z };
		ParticleTemp->SetVisibility(true);
		ParticleTemp->SetBeamSourcePoint(0, StartBeam, 0);
		ParticleTemp->SetBeamEndPoint(0, EndBeam);
		Beams.Add(ParticleTemp);
		GenerateRecursive(Start+End, Distance/(Lvl+1), Height + Intermission * Direction,-1,Lvl+1);
		GenerateRecursive(Start+End, Distance/(Lvl+1), Height + Intermission * Direction,1,Lvl+1);
	}

}







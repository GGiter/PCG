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
	
}

// Called every frame
void AFractalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AFractalActor::Generate()
{
	

}
void AFractalActor::SetFractalSize(FVector NewSize)
{
	Size = NewSize;
}

TArray<TPair<FVector, FVector>>  AFractalActor::GetPDIIntstuctions()
{
	return PDIInstructions;
}


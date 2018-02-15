// Fill out your copyright notice in the Description page of Project Settings.

#include "FractalUI.h"
#include "FractalActor.h"



void UFractalUI::SetIsFractalSet(bool NewIsFractalSet)
{
	IsFractalSet = NewIsFractalSet;
}

void UFractalUI::CallGenerate()
{
	FractalRef->Generate();
}
void UFractalUI::SetFractalRef(TSubclassOf<AFractalActor> NewFractalClass,FVector Location, FRotator Rotation)
{
	FractalClass = NewFractalClass;
	SetUp(Location,Rotation);
}
void UFractalUI::SetUp(FVector Location, FRotator Rotation)
{
	if (FractalRef)
	{
		FractalRef->Destroy();
	}
	FActorSpawnParameters SpawnInfo;
	FractalRef=GetWorld()->SpawnActor<AFractalActor>(FractalClass,Location, Rotation, SpawnInfo);
}
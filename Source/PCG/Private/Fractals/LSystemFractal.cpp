// Fill out your copyright notice in the Description page of Project Settings.

#include "LSystemFractal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


void ALSystemFractal::SpawnLine(FVector Location1,FVector Location2)
{
	
	UParticleSystemComponent* ParticleTemp;
	ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FractalPSC, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false);
	ParticleTemp->SetVisibility(true);
	ParticleTemp->SetBeamSourcePoint(0,Location1, 0);
	ParticleTemp->SetBeamEndPoint(0, Location2);
	Beams.Add(ParticleTemp);
	SetLocation(Location2);
	

	

}
void ALSystemFractal::BeginPlay()
{
	Super::BeginPlay();
	LoadRules();

}
void ALSystemFractal::Generate()
{
		Super::Generate();
		FString Sequence = GenerateSequence(NumOfIterations);
		ResetTurtle();
		PenDown();
		for (int32 i = 0; i < Sequence.Len(); ++i)
		{
			
			if (Sequence[i] == 'F')
			{
				SpawnLine(GetLocation(),Forward(LineLength));
			}
			else if (Sequence[i] == '-')
			{
				TurnLeft(InputAngleZ);
			}
			else if (Sequence[i] == '+')
			{
				TurnRight(InputAngleZ);
			}
			else if (Sequence[i] == '&')
			{
				PitchDown(InputAngleY);
			}
			else if (Sequence[i] == '^')
			{
				PitchUp(InputAngleY);
			}
			else if (Sequence[i] == '<')
			{
				RollLeft(InputAngleY);
			}
			else if (Sequence[i] == '>')
			{
				RollRight(InputAngleY);
			}
			else if (Sequence[i] == '[')
			{
				Push(GetLocation(), GetDirection());
			}
			else if (Sequence[i] == ']')
			{
				FTurtleTriplet Temp = Pop();
				PenUp();
				SetPosition(Temp.Location);
				SetDirection(Temp.Angle);
				PenDown();
			}
		}
	
}
void ALSystemFractal::LoadRules()
{
	for (FPair TempPair : InputRules)
	{
		Rules.Add(TempPair.First[0], TempPair.Second);
	}
}
FString ALSystemFractal::GenerateSequence(int32 Length)
{
	FString Sequence = Axiom;
	for (int32 i = 0; i < Length; ++i)
	{
		FString Complement = "";
		for (int32 j = 0; j < Sequence.Len(); ++j)
		{
			
			if (Rules.Find(Sequence[j])!=nullptr)
			{
				
				Complement += Rules[Sequence[j]];
			}
			else
			{
				Complement += Sequence[j];
			}
		}
		Sequence = Complement;
	}

	return Sequence;
}


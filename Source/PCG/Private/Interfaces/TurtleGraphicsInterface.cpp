// Fill out your copyright notice in the Description page of Project Settings.

#include "TurtleGraphicsInterface.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

void ITurtleGraphicsInterface::TurnLeft(float Degree)
{
	TurnRight(-Degree);
}

void ITurtleGraphicsInterface::TurnRight(float  Degree)
{
	FVector Axis = Direction;
	Axis=FVector::CrossProduct(Axis, Right);
	Direction=Direction.RotateAngleAxis(Degree, Axis);
	Right=Right.RotateAngleAxis(Degree, Axis);
	Direction.Normalize(0.0f);
	Right.Normalize(0.0f);
	
}

void ITurtleGraphicsInterface::PitchUp(float  Degree)
{
	Direction=Direction.RotateAngleAxis(Degree, Right);
	Direction.Normalize(0.0f);
}

void ITurtleGraphicsInterface::PitchDown(float  Degree)
{
	PitchUp(-Degree);
}

void ITurtleGraphicsInterface::RollRight(float  Degree)
{
	Right=Right.RotateAngleAxis(Degree, Direction);
	Right.Normalize(0.0f);
}

void ITurtleGraphicsInterface::RollLeft(float  Degree)
{
	RollRight(-Degree);
}


FVector ITurtleGraphicsInterface::Forward(const float & Distance)
{
	FVector Temp = Direction;
	Temp.Normalize(0.0f);
	return Location + Distance*Temp;
}

void ITurtleGraphicsInterface::Back(const float & Distance)
{
	Forward(-Distance);
}

void ITurtleGraphicsInterface::PenDown()
{
	PenIsDown = true;
}

void ITurtleGraphicsInterface::PenUp()
{
	PenIsDown = false;
}




void ITurtleGraphicsInterface::Push(FVector Location, FVector Direction)
{
	Stack.Push(FTurtleTriplet(Location, Direction));
}

FTurtleTriplet ITurtleGraphicsInterface::Pop()
{
	return Stack.Pop();
}

float ITurtleGraphicsInterface::DistanceTo(FVector NewLocation)
{
	return FMath::Sqrt(FMath::Square(Location.X - NewLocation.X) + FMath::Square(Location.Y - NewLocation.Y)+ FMath::Square(Location.Z-NewLocation.Z));
}

float ITurtleGraphicsInterface::AngleTo(FVector NewLocation)
{
	//float AbsAngle = FMath::RadiansToDegrees(FMath::Atan2(FVector::CrossProduct(NewLocation,Location).Normalize(), FVector::DotProduct(NewLocation,Location)));
	//float NewAngle = FMath::Fmod((AbsAngle - Angle.X + 360.0f), 360.0f);
	return 0.0f;
}


void ITurtleGraphicsInterface::TurnToward(FVector NewLocation, float Degree)
{
	float NewAngle = AngleTo(NewLocation);
	if (NewAngle < 180.0f)
	{
		//Angle.X += Degree;
	}
	else
	{
		//Angle.X -= Degree;
	}
}

void ITurtleGraphicsInterface::Face(const float NewAnglee)
{
	//Angle.X = NewAnglee;
}

void ITurtleGraphicsInterface::SetPosition(FVector NewLocation)
{
	Location = NewLocation;
}

void ITurtleGraphicsInterface::SetDirection(FVector NewDirection)
{
	Direction = NewDirection;
}

FVector ITurtleGraphicsInterface::GetDirection() const
{
	return Direction;
}

FVector ITurtleGraphicsInterface::GetLocation() const
{
	return Location;
}

void ITurtleGraphicsInterface::SetLocation(FVector NewLocation)
{
	Location = NewLocation;
}

void ITurtleGraphicsInterface::ResetTurtle()
{
	Location = { 0.0f,0.0f,0.0f };
	Direction = { 0.0f,1.0f,0.0f };
	Right = { 1.0f,0.0f,0.0f };
	bool PenIsDown = true;
	Stack.Empty();
}






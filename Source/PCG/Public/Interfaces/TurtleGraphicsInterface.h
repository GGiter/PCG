// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TurtleGraphicsInterface.generated.h"

UINTERFACE(Blueprintable)
class UTurtleGraphicsInterface : public UInterface
{
	GENERATED_BODY()
};
USTRUCT(BlueprintType)
struct FTurtleTriplet
{
	GENERATED_USTRUCT_BODY()
	FVector Location;
	FVector Angle;
	FTurtleTriplet()
	{

	}
	FTurtleTriplet(FVector NewLocation,FVector NewAngle)
	{
		Location = NewLocation;
		Angle = NewAngle;
	}
};
class ITurtleGraphicsInterface
{
	GENERATED_BODY()

public:

	void TurnLeft(float Degree);
	void TurnRight(float Degree);
	void PitchUp(float Degree);
	void PitchDown(float Degree);
	void RollRight(float Degree);
	void RollLeft(float Degree);
	FVector Forward(const float& Distance);
	void Back(const float& Distance);
	void PenDown();
	void PenUp();
	void Push(FVector Location,FVector Direction);
	FTurtleTriplet Pop();
	float DistanceTo(FVector NewLocation);
	float AngleTo(FVector NewLocation);
	void TurnToward(FVector NewLocation, float Degree);
	void Face(const float NewAnglee);
	void SetPosition(FVector NewLocation);
	void SetDirection(FVector NewDirection);
	FVector GetDirection() const;
	FVector GetLocation() const;
	void SetLocation(FVector NewLocation);
	void ResetTurtle();

private:
	FVector Location = { 0.0f,0.0f,0.0f };
	FVector Direction = { 0.0f,1.0f,0.0f };
	FVector Right = { 1.0f,0.0f,0.0f };
	bool PenIsDown = true;
	TArray < FTurtleTriplet> Stack;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/ThreadSafeCounter.h"


/**
 * 
 */


class SCLeaf
{
	

public:

	SCLeaf();
	void Reached();
	FVector GetPosition() const;
	bool GetReached();
private:
	FVector Position;
	bool bReached = false;
	float Width = 600.0f;
	float Height = 900.0f;


};


class SCBranch
{
	

public:
	// Sets default values for this actor's properties
	SCBranch(FVector V, FVector D);
	SCBranch(SCBranch * P);
	SCBranch();

	void Reset();
	FVector Next();
	FVector GetDirection() const;
	FVector GetPosition() const;
	int32 GetCount() const;
	SCBranch* GetParent() const;
	void SetDirection(FVector NewDirection);
	void SetCount(int32 NewCount);
private:
	SCBranch *Parent;
	FVector Position;
	FVector Direction;
	int32 Count = 0;
	FVector SaveDirection;
	float Len = 5;


};

class FSCTree : public FRunnable
{
	static FSCTree * Runnable;

	FRunnableThread* Thread;

	TArray<TPair<FVector,FVector> >* Locations;

	FThreadSafeCounter StopTaskCounter;
public:
	FSCTree(TArray<TPair<FVector, FVector> >& LocationArray);
	~FSCTree();
	bool CloseEnough(SCBranch* B);
	void Grow();
	void Draw();
	bool IsFinished() const
	{
		return bFinished;
	}
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();

	static FSCTree* JoyInit(TArray<TPair<FVector, FVector> >& LocationArray);

	static void Shutdown();

	static bool IsThreadFinished();
private:
	TArray<SCBranch*> Branches;
	TArray<SCLeaf*> Leaves;
	float Height= 900.0f, Width= 600.0f;;
	float Max_Dist= 200.0f, Min_Dist =5.0f;
	//void SpawnLine(FVector Position1, FVector Position2);
	bool bFinished = false;
};

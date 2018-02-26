// Fill out your copyright notice in the Description page of Project Settings.

#include "SCTree.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"

FSCTree* FSCTree::Runnable = NULL;

SCLeaf::SCLeaf()
{
	Position = FMath::VRand();
	Position *= FMath::FRandRange(0, Width / 2);
	Position.Y -= Height / 4;
}

void SCLeaf::Reached()
{
	bReached = true;
}

FVector SCLeaf::GetPosition() const
{
	return Position;
}

bool SCLeaf::GetReached()
{
	return bReached;
}

SCBranch::SCBranch(FVector V, FVector D)
{
	Parent = nullptr;
	Position = V;
	Direction = D;
	SaveDirection = Direction;
}
SCBranch::SCBranch(SCBranch * P)
{
	Parent = P;
	Position = Parent->Next();
	Direction = Parent->GetDirection();
	SaveDirection = Direction;
}

SCBranch::SCBranch()
{
}


void SCBranch::Reset()
{
	Count = 0;
	Direction = SaveDirection;
}

FVector SCBranch::Next()
{
	FVector V = Direction*Len;
	FVector Next = Position + V;
	return Next;
}

FVector SCBranch::GetDirection() const
{
	return Direction;
}

FVector SCBranch::GetPosition() const
{
	return Position;
}

int32 SCBranch::GetCount() const
{
	return Count;
}

SCBranch * SCBranch::GetParent() const
{
	return Parent;
}

void SCBranch::SetDirection(FVector NewDirection)
{
	Direction = NewDirection;
}

void SCBranch::SetCount(int32 NewCount)
{
	Count = NewCount;
}





FSCTree::FSCTree(TArray<TPair<FVector, FVector> >& LocationArray)
{
	Locations = &LocationArray;
	Thread = FRunnableThread::Create(this, TEXT("FSCTreeQ"), 0, TPri_BelowNormal);
}
FSCTree::~FSCTree()
{
	delete Thread;
	Thread = nullptr;
}
bool FSCTree::CloseEnough(SCBranch* B)
{
	for (SCLeaf* L : Leaves)
	{
		float D = FVector::Dist(B->GetPosition(), L->GetPosition());
		if (D < Max_Dist)
		{
			return true;
		}
	}
	return false;
}
void FSCTree::Draw()
{
	
}
/*
void ASCTree::SpawnLine(FVector Position1, FVector Position2)
{
	UParticleSystemComponent* ParticleTemp;
	ParticleTemp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FractalPSC, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false);
	ParticleTemp->SetVisibility(true);
	ParticleTemp->SetBeamSourcePoint(0, Position1, 0);
	ParticleTemp->SetBeamEndPoint(0, Position2);
	Beams.Add(ParticleTemp);
}
*/
bool FSCTree::Init()
{
	for (int32 i = 0; i < 4000; ++i)
	{
		Leaves.Add(new SCLeaf());
	}
	SCBranch* BRoot = new SCBranch(FVector(0, Height / 2, 0), FVector(0, -1, 0));
	Branches.Add(BRoot);
	SCBranch* Current = new SCBranch(BRoot);

	while (!CloseEnough(Current)) {
		SCBranch* Trunk = new SCBranch(Current);
		Branches.Add(Trunk);
		Current = Trunk;
	}
	return true;
}
uint32 FSCTree::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		for (SCLeaf* L : Leaves)
		{
			SCBranch* Closest = nullptr;
			FVector ClosestDirection(0.0f, 0.0f, 0.0f);
			float Record = -1;
			for (SCBranch* B : Branches)
			{
				FVector Direction = L->GetPosition() - B->GetPosition();
				float D = Direction.Size();
				if (D < Min_Dist)
				{
					L->Reached();
					Closest = nullptr;
					break;
				}
				else if (D > Max_Dist)
				{

				}
				else if (Closest == nullptr || D < Record)
				{
					Closest = B;
					ClosestDirection = Direction;
					Record = D;

				}
				if (Closest != nullptr)
				{
					ClosestDirection.Normalize();
					Closest->SetDirection(Closest->GetDirection() + ClosestDirection);
					Closest->SetCount(Closest->GetCount() + 1);
				}
			}
		}

		for (int32 i = Leaves.Num() - 1; i >= 0; --i)
		{
			if (Leaves[i]->GetReached() == true)
			{
				Leaves.RemoveAt(i);
			}
		}

		for (int32 i = Branches.Num() - 1; i >= 0; --i)
		{
			SCBranch* B = Branches[i];
			if (B->GetCount() > 0)
			{
				B->SetDirection(B->GetDirection() / B->GetCount());
				float X = FMath::FRand();
				float Y = FMath::FRand();
				FVector Rand(X, Y, 0.0f);
				Rand = { Rand.X*0.3f / Rand.Size(),Rand.Y*0.3f / Rand.Size() ,0.0f };
				B->SetDirection(B->GetDirection() + Rand);
				FVector Norm = B->GetDirection();
				Norm.Normalize();
				B->SetDirection(Norm);
				SCBranch* NewB = new SCBranch(B);
				Branches.Add(NewB);
				B->Reset();

			}
		}
		

		for (int32 i = 0; i < Branches.Num(); ++i)
		{
			SCBranch* B = Branches[i];
			if (B->GetParent() != nullptr)
			{
				//SpawnLine(B->GetPosition(), B->GetParent()->GetPosition());
				Locations->Add(TPair<FVector, FVector>(B->GetPosition(), B->GetParent()->GetPosition()));
			}
		}
		bFinished = true;
	}
	return 0;
}
void FSCTree::Stop()
{
	StopTaskCounter.Increment();
}

void FSCTree::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

FSCTree * FSCTree::JoyInit(TArray<TPair<FVector, FVector> >& LocationArray)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSCTree(LocationArray);
	}
	return Runnable;
}

void FSCTree::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FSCTree::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}



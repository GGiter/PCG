// Fill out your copyright notice in the Description page of Project Settings.

#include "FractalDrawComponent.h"
#include "Runtime/Engine/Public/SceneManagement.h"
#include "Public/Fractals/FractalActor.h"

class FFractalProxy : public FPrimitiveSceneProxy
{

public:
	AFractalActor* FractalActorRef;
	FFractalProxy(UPrimitiveComponent* Component, AFractalActor* NewFractalActorRef)
		: FPrimitiveSceneProxy(Component)
	{
		FractalActorRef = NewFractalActorRef;
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;

		return Result;
	}

	virtual uint32 GetMemoryFootprint(void) const override
	{
		return(sizeof(*this) + GetAllocatedSize());
	}

	virtual bool CanBeOccluded() const override
	{
		return false;
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView *>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override
	{
		TArray<TPair<FVector, FVector> > Instructions = FractalActorRef->GetPDIIntstuctions();
		

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

			
		
			for (TPair<FVector,FVector> Instruction : Instructions)
			{

				PDI->DrawLine(Instruction.Key, Instruction.Value, FColor::Yellow, SDPG_World, 1.f);
				
			}
		}
	}
};
FPrimitiveSceneProxy* UFractalDrawComponent::CreateSceneProxy()
{
	return new FFractalProxy(this, FractalActorRef);
}



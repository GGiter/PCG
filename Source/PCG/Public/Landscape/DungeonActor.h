// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonAlg.h"
#include "RuntimeMeshComponent.h"
#include "DungeonActor.generated.h"

UCLASS()
class PCG_API ADungeonActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonActor();
	UFUNCTION(BLueprintCallable)
		void Generate();
	UFUNCTION(BlueprintImplementableEvent)
		void SetColors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URuntimeMeshComponent* RuntimeMesh;
	void Render();
	void GenerateGround(TArray<FVector> Vertices, int32 SectionIndex);
	void RenderHallways(TArray<FVector> NewVertices, int32 SectionIndex);
	TArray<TArray<FVector> > PointLists;
	bool bC = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 NumOfSections;
	TArray<TArray<int32>> NumOfTypes;
	
};

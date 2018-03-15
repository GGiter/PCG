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
	UFUNCTION(BlueprintPure)
		float GetHallwayWidth() const;
	UFUNCTION(BlueprintPure)
		int32 GetNumberOfRooms() const;
	UFUNCTION(BlueprintPure)
		float GetSphereRadius() const;
	UFUNCTION(BlueprintCallable)
		void SetHallwayWidth(float NewHallwayWidth);
	UFUNCTION(BlueprintCallable)
		void SetNumberOfRooms(int32 NewNumberOfRooms);
	UFUNCTION(BlueprintCallable)
		void SetSphereRadius(float NewSphereRadius);
	

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
	void RenderRoom(TArray<FEdge> Edges,TArray<FVector> NewVertices, int32 SectionIndex);
	TArray<TArray<FVector> > PointLists;
	bool bC = true;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 NumOfSections;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float HallwayWidth=90.0f;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 NumberOfRooms = 150;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float SphereRadius = 700.0f;
	TArray<TArray<int32>> NumOfTypes;
	TArray<TArray<FEdge>> RoomsBorders;
	TArray<TArray<FEdge>> HubsBorders;
	TArray<int32> HubsIndexes, RoomsIndexes, HallwaysIndexes, GroundIndexes; //ColorIndexes
};

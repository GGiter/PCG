// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleGraphicsInterface.h"
#include "FractalDrawComponent.h"
#include "FractalActor.generated.h"

UCLASS()
class PCG_API AFractalActor : public AActor, public ITurtleGraphicsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFractalActor();
	UFUNCTION(BlueprintCallable)
	virtual void Generate();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE
		UFUNCTION(BlueprintPure,Category=Props)
		FString GetFractalName() const { return FractalName; }
	FORCEINLINE
		UFUNCTION(BlueprintPure,Category=Props)
		FVector GetFractalSize() const { return Size; }
	UFUNCTION(BlueprintCallable, Category = Props)
		void SetFractalSize(FVector NewSize);
	TArray<TPair<FVector, FVector>>  GetPDIIntstuctions();
	UFUNCTION(BlueprintPure, Category = Camera)
		FVector GetCenterPoint();
	UFUNCTION(BlueprintCallable, Category = Beams)
		void DestroyBeams();
	UFUNCTION(BlueprintCallable, Category = Beams)
		TArray<class UParticleSystemComponent*> GetBeams();
	UFUNCTION(BlueprintPure)
		int32 GetNumOfIterations();
	UFUNCTION(BlueprintCallable)
		void SetNumOfIterations(int32 NewNumOfIterations);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UHierarchicalInstancedStaticMeshComponent* HISMActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UFractalDrawComponent* DrawComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector Size;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		float LineLength;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"))
		UParticleSystem* FractalPSC;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class UParticleSystemComponent*> Beams;
	TArray<TPair<FVector, FVector>> PDIInstructions;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		int32 NumOfIterations = 4;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Props, meta = (AllowPrivateAccess = "true"))
		FString FractalName;
	
	
	
	
};

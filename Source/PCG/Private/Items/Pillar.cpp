// Fill out your copyright notice in the Description page of Project Settings.

#include "Pillar.h"
#include "FractalActor.h"
#include "PCGPlayer.h"
#include "FractalUI.h"

APillar::APillar()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Scene->SetupAttachment(RootComponent);
}
void APillar::BeginPlay()
{
	Super::BeginPlay();
	
}
void APillar::OnInteract_Implementation(AActor*Caller)
{
	APCGPlayer* PlayerRef = Cast<APCGPlayer>(Caller);
	if (PlayerRef)
	{
		UFractalUI* FractalUIRef = Cast<UFractalUI>(PlayerRef->GetPlayerHUD());
		if (FractalUIRef)
		{
			if (FractalClass)
			{
				FractalUIRef->SetFractalRef(FractalClass,Scene->GetComponentLocation(),Scene->GetComponentRotation());
				FractalUIRef->SetIsFractalSet(true);
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Casting UI Failed"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting Player Failed"));
	}
}


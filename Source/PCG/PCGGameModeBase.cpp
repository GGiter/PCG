// Fill out your copyright notice in the Description page of Project Settings.

#include "PCGGameModeBase.h"

void APCGGameModeBase::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	double Secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 Seed = (int32)(((int64)Secs) % INT_MAX);
	FMath::RandInit(Seed);
	
}

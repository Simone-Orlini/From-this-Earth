// Fill out your copyright notice in the Description page of Project Settings.


#include "GridFunctionLibrary.h"

FVector UGridFunctionLibrary::SnapVectorToVector(FVector Location, FVector GridSize)
{
	Location.X = FMath::GridSnap<float>(Location.X, GridSize.X);;
	Location.Y = FMath::GridSnap<float>(Location.Y, GridSize.Y);;
	Location.Z = FMath::GridSnap<float>(Location.Z, GridSize.Z);;
	
	return Location;
}

FVector UGridFunctionLibrary::SnapVectorToVector(FVector Location, FVector2D GridSize)
{
	Location.X = FMath::GridSnap<float>(Location.X, GridSize.X);;
	Location.Y = FMath::GridSnap<float>(Location.Y, GridSize.Y);;
	Location.Z = FMath::GridSnap<float>(Location.Z, 0);;
	
	return Location;
}


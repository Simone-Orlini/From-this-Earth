// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridFunctionLibrary.h"

using GridFL = UGridFunctionLibrary;

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SpawnGrid();
}

void AGrid::SpawnGrid()
{
	FVector center;
	FVector bottomLeft;
	CalculateCenterAndBottomLeft(center, bottomLeft);
	FVector offset;
	offset.X = GridTileSize.X * 0.5f;
	offset.Y = GridTileSize.Y * 0.5f;
	offset.Y = GridTileSize.Z * -0.05f;
	
	FlushPersistentDebugLines(GetWorld());
	FVector tileLocation = GetActorLocation();
	
	if (DrawFullGrid)
	{
		for (int i = 0; i < GridSize.X * GridSize.Y; i++)
		{
			int x = i % GridSize.X;
			int y = i / GridSize.X;
			
			tileLocation = bottomLeft + FVector(GridTileSize.X * x, GridTileSize.Y * y, tileLocation.Z);
			if (UseEnv)
			{
				// Trace for the ground
			}
			else
			{
				tileLocation -= offset;
				FPlane plane = FPlane(FVector::UpVector, 0.0f);
				DrawDebugSolidPlane(GetWorld(), plane, tileLocation, FVector2D(GridTileSize.X * 0.5f, GridTileSize.Y * 0.5f), LineColor, true, -1.0f, 0);
				
				//FVector lineEnd = tileLocation;
				//lineEnd.X += tileLocation.X;	
			// 	DrawDebugLine(nullptr, tileLocation, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
			// 	lineEnd.X = 0;
			// 	lineEnd.Y += tileLocation.Y;
			// 	DrawDebugLine(nullptr, tileLocation, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
			}
			
			
		}
	}
	else
	{
		tileLocation = bottomLeft - offset;
		FVector lineEnd = tileLocation;
		lineEnd.X += GridTileSize.X * GridSize.X;
		DrawDebugLine(GetWorld(), tileLocation, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
		
		lineEnd = tileLocation;
		lineEnd.Y += GridTileSize.Y * GridSize.Y;
		DrawDebugLine(GetWorld(), tileLocation, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
		
		FVector lineStart = tileLocation;
		lineStart.X += GridTileSize.X * GridSize.X;
		lineEnd = lineStart;
		lineEnd.Y += GridTileSize.Y * GridSize.Y;
		DrawDebugLine(GetWorld(), lineStart, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
		
		lineStart = tileLocation;
		lineStart.Y += GridTileSize.Y * GridSize.Y;
		lineEnd = lineStart;
		lineEnd.X += GridTileSize.X * GridSize.X;
		DrawDebugLine(GetWorld(), lineStart, lineEnd, LineColor, true, -1.0f, 0, LineThickness);
		
	}
	
}

void AGrid::CalculateCenterAndBottomLeft(FVector& center, FVector& bottomLeft)
{
	center = GridFL::SnapVectorToVector(GetActorLocation(), GridTileSize);
	FVector temp;
	temp.X = GridFL::IsEven(GridSize.X) ? 0 : 1;
	temp.Y = GridFL::IsEven(GridSize.Y) ? 0 : 1;
	
	FVector Extents = FVector(
	(GridSize.X - temp.X) * 0.5f * GridTileSize.X,
	(GridSize.Y - temp.Y) * 0.5f * GridTileSize.Y,
	0.0f);
	
	bottomLeft = center - Extents;
}


// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

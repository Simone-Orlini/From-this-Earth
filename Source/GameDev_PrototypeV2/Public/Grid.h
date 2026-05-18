// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class GAMEDEV_PROTOTYPEV2_API AGrid : public AActor
{
	GENERATED_BODY()
	
	
	
	
	
	
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Debug parameters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Debug")
	bool UseEnv;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Debug")
	bool DrawFullGrid;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Debug")
	FColor LineColor = FColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Debug")
	float LineThickness = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Parameters")
	FVector GridTileSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Parameters")
	FIntPoint GridSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
private:
	void SpawnGrid();
	void CalculateCenterAndBottomLeft(FVector& Center, FVector& BottomLeft);
};

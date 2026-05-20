// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.h"
#include "UObject/Interface.h"
#include "GamemodeAImanger.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGamemodeAImanger : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEDEV_PROTOTYPEV2_API IGamemodeAImanger
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GMAI")
	FTileData GetTileData(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GMAI")
	TArray<FTileData> GetTilesFromBFS(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GMAI")
	TArray<FIntPoint> GetPointsFromPath(AActor* Actor, const FTileData& Tile);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GMAI")
	TArray<AActor*> GetUnitsAttackable(const FTileData& TileData, AActor* Actor);
	
};

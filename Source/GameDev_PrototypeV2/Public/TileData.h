#pragma once
#include "CoreMinimal.h"
#include "TileData.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Empty,
	ToReach,
	Bush,
	Mud,
	Fire,
};


UENUM(BlueprintType)
enum class ETileStatus : uint8
{
	// the elements are in order of importans
	Highlight, // It means when a tile is under the cursor
	Attack,
	Movement,
	Hovered,
	None,
};

USTRUCT(BlueprintType)
struct FGridTileData
{
	GENERATED_BODY();
	
	FGridTileData():Index(FIntPoint(-1, -1)), Transform(FTransform()), TileType(ETileType::Empty)
	{
		
	};
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FIntPoint Index;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FTransform Transform;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	ETileType TileType;
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY();
	
	FTileData():GridTileData(), bIsWalkable(true), Actor(nullptr), Outline(nullptr), VFX(nullptr), Item(nullptr)
	{
		Neighbors.Empty();
		TileStatuses.AddUnique(ETileStatus::None);
	}
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FGridTileData GridTileData;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TArray<FIntPoint> Neighbors;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	bool bIsWalkable;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	AActor* Actor;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TArray<ETileStatus> TileStatuses;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	UStaticMesh* Outline;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	class UNiagaraSystem* VFX;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	class AItem* Item;
	
	int32 GetAICost() const
	{
		switch (GridTileData.TileType)
		{
			// Valori monentanei da tunare
			default:
			case ETileType::Empty:
				return 5;
			case ETileType::ToReach:
				return 0; // The AI cant reach the tile ToReach
			case ETileType::Bush:
				return 7;
			case ETileType::Mud:
				return 3;
			case ETileType::Fire:
				return 1;
		}
	}
	
	bool IsOccupied() const {return Actor != nullptr;}
};




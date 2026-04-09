#pragma once
#include "CoreMinimal.h"
#include "TileData.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Empty,
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
struct FTileData
{
	GENERATED_BODY();
	
	FTileData():Index(FIntPoint(-1, -1)), bIsWalkable(true), Actor(nullptr), TileType(ETileType::Empty), Outline(nullptr), VFX(nullptr)
	{
		Neighbors.Empty();
		TileStatus.Add(ETileStatus::None);
	}
	
	FIntPoint Index;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TArray<FIntPoint> Neighbors;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	bool bIsWalkable;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	AActor* Actor;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	ETileType TileType;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TArray<ETileStatus> TileStatus;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	UStaticMesh* Outline;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	class UNiagaraSystem* VFX;
	
	int32 AICost() const
	{
		switch (TileType)
		{
			// Valori monentanei da tunare
			default:
			case ETileType::Empty:
				return 0;
			case ETileType::Bush:
				return 2;
			case ETileType::Mud:
				return -2;
			case ETileType::Fire:
				return -5;
		}
	}
	
	bool IsOccupied() const {return Actor != nullptr;}
};
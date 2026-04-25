// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "TileData.h"
#include "GamemodeAImanger.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

struct FTileInfo
{
	FTileInfo() :ActorToAttack(nullptr), FinalCost(-1.0f)
	{

	}
	AActor* ActorToAttack;
	float FinalCost;
};

// Sets default values for this component's properties
UAIManager::UAIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

PRAGMA_DISABLE_OPTIMIZATION
void UAIManager::StartLogic(const TArray<AActor*>& Enemies)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

	TArray<FIntPoint> tileOccupated;
	TArray<AActor*> pUnitsAttacked;

	for (AActor* cEnemy : Enemies)
	{
		TArray<FTileData> tiles;
		tiles.Add(IGamemodeAImanger::Execute_GetTileData(GameMode, cEnemy));
		tiles.Append(IGamemodeAImanger::Execute_GetTilesFromBFS(GameMode, cEnemy));

		TMap<FIntPoint, FTileInfo> tilesInfo;
		bool bCanAttackWithoutMove = false;
		int32 i = 0;

		for (FTileData& cTile : tiles)
		{
			FTileInfo info;

			if (i != 0 && (!cTile.bIsWalkable || cTile.IsOccupied() || tileOccupated.Contains(cTile.GridTileData.Index)))
				continue;



			float tileCost = cTile.GetAICost() / 7.0f; // 7 is the maxCost - MAGICNUMBER BLEH
			float distCost = 0.2f; //AGameModeBase.Path(cEnemy, cTile);
			if (distCost < 0.001f || i == 0)
				distCost = 1.5f;

			float attackCost = 0.0f;

			if (!bCanAttackWithoutMove)
			{
				CalculateAttackCost(cTile, cEnemy, pUnitsAttacked, info, attackCost);

				if (i == 0 && attackCost > 0.0f)
				{
					bCanAttackWithoutMove = true;
				}
			}

			float finalCost = tileCost * tileWeight +
				distCost * distWeight +
				attackCost * attackWeight;

			info.FinalCost = finalCost;
			tilesInfo.Add(cTile.GridTileData.Index, info);

			i++;
		}

		TArray<FIntPoint> bestTiles;
		float valueToCompare = 0.0f;

		for (auto cKeyValue : tilesInfo)
		{
			float value = cKeyValue.Value.FinalCost;

			if (value > valueToCompare + 0.001f) // 0.001f is a threshold
			{
				valueToCompare = value;
				bestTiles.Empty();
				bestTiles.Add(cKeyValue.Key);
			}
			else if (FMath::IsNearlyEqual(value, valueToCompare, 0.001f))
			{
				bestTiles.Add(cKeyValue.Key);
			}
		}

		int32 randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
		FIntPoint rBestTile = bestTiles[randomIndex];

		FAIData data;

		AActor* unitToAttack = tilesInfo.Find(rBestTile)->ActorToAttack;

		if (rBestTile == IGamemodeAImanger::Execute_GetTileData(GameMode, cEnemy).GridTileData.Index)
		{
			if (unitToAttack)
			{
				data.ActorToAttack = unitToAttack;

				if (bestTiles.Num() > 1)
				{
					bestTiles.Remove(rBestTile);
					randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
					rBestTile = bestTiles[randomIndex];

					data.ActionType = EActionType::AttackToMove;
					data.TileToReach = rBestTile;
				}
				else
				{
					// Sarebbe da rivalutare gli altri tile che hanno un peso minore
					data.ActionType = EActionType::Attack;
				}
			}
			else
			{
				data.ActorToAttack = nullptr;

				if (bestTiles.Num() > 1)
				{
					bestTiles.Remove(rBestTile);
					randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
					rBestTile = bestTiles[randomIndex];

					data.ActionType = EActionType::Move;
					data.TileToReach = rBestTile;
				}
				else
				{
					// Sarebbe da rivalutare gli altri tile che hanno un peso minore
					data.ActionType = EActionType::Stay;
				}
			}
		}
		else
		{
			if (unitToAttack)
			{
				data.ActionType = EActionType::MoveToAttack;
				data.TileToReach = rBestTile;
				data.ActorToAttack = unitToAttack;
			}
			else
			{
				data.ActionType = EActionType::Move;
				data.TileToReach = rBestTile;
				data.ActorToAttack = nullptr;
			}

		}

		AIDatas.Add(cEnemy) = data;
	}
}
PRAGMA_DISABLE_OPTIMIZATION

void UAIManager::CalculateAttackCost(const FTileData& TileData, AActor* Enemy, const TArray<AActor*>& PUnitsAttacked, FTileInfo& OutInfo, float& OutAttackCost)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

	TArray<AActor*> unitsAttackable = IGamemodeAImanger::Execute_GetUnitsAttackable(GameMode, TileData, Enemy); // Gamemode.CheckAttack(tiledata, Enemy)

	for (AActor* cUnit : unitsAttackable)
	{
		if (PUnitsAttacked.Contains(cUnit))
		{
			OutInfo.ActorToAttack = nullptr;
			continue;
		}

		float unitDistCost = IGamemodeAImanger::Execute_GetPointsFromPath(GameMode, cUnit, TileData).Num() / 3.0f; // Magic number da sostiture con il maxAttackRange
		// float damageCost = 1 - 0.0f; //(FMath::Max(0, cUnit->GetComponentByClass(CombatComponent).currentHealth - Enemy.GetComponentByClass(WeaponCompoennt).damage))
		float tempAttackCost = unitDistCost * unitDistWeight;
		// damageCost * damageWeight;

		if (tempAttackCost > OutAttackCost)
		{
			OutAttackCost = tempAttackCost;
			OutInfo.ActorToAttack = cUnit;
		}
	}
}


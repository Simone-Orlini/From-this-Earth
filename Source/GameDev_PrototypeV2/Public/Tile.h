// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class GAMEDEV_PROTOTYPEV2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	
	UPROPERTY(BlueprintReadOnly, Category="Grid|Tile")
	FTileData tileData;	
	
private:
	UPROPERTY(EditAnywhere, Category="Grid|Tile")
	FVector spawnLocEnvGraphicElement = FVector(0,0,0);
	
	UPROPERTY(EditAnywhere, Category="Grid|Tile")
	UStaticMeshComponent* outline;
	
	UPROPERTY(EditAnywhere, Category="Grid|Tile")
	class UNiagaraComponent* VFX;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile")
	void ApplyEffect();
	
	UFUNCTION(BlueprintCallable,  Category="Grid|Tile")
	void UnitEnterTile();
	
	UFUNCTION(BlueprintCallable,  Category="Grid|Tile")
	void SpawnTile(FVector location, const FTileData& _tileData);
	
	UFUNCTION(BlueprintCallable)
	void InitTileData(const FTileData& _tileData);
	
	UFUNCTION(BlueprintCallable)
	void UpdateOutline();
	
	UFUNCTION(BlueprintCallable)
	void AddOutlineStatus(ETileStatus newStatus);
	
	UFUNCTION(BlueprintCallable)
	void RemoveOutlineStatus(ETileStatus oldStatus);


};

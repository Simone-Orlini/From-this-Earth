// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.h"
#include "GamemodeData.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class GAMEDEV_PROTOTYPEV2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	
	UPROPERTY(BlueprintReadWrite, Category="Grid|Tile")
	FTileData tileData;	
	
private:
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Tile")
	FVector spawnLocEnvGraphicElement = FVector(0,0,0);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Tile|Visual")
	UStaticMeshComponent* VisualElementMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Tile|Visual")
	TMap<EMaskType, UStaticMesh*> VisualTileTypeElements; // Da cambiare con il tipo generico
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Tile")
	UStaticMeshComponent* outline;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Tile")
	class UNiagaraComponent* VFX;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile")
	void ApplyEffect();
	
	UFUNCTION(BlueprintCallable,  Category="Grid|Tile")
	void UnitEnterTile(AActor* Actor);
	
	UFUNCTION(BlueprintCallable,  Category="Grid|Tile")
	void SpawnTile(FTileData _tileData, TArray<UStaticMesh*> meshes);
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile")
	void InitTileData(FTileData _tileData);
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile|Visual")
	void UpdateOutline();
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile|Visual")
	void AddOutlineStatus(ETileStatus newStatus);
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile|Visual")
	void RemoveOutlineStatus(ETileStatus oldStatus);
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile|Visual")
	void EnableVFX();
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile")
	void InitVisualElement(TArray<UStaticMesh*> meshes);
	
	UFUNCTION(BlueprintCallable, Category="Grid|Tile|Visual")
	void SpawnVisualElement(EMaskType MaskType);
};

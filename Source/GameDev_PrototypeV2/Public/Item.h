// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"

UCLASS()
class GAMEDEV_PROTOTYPEV2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float speed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Item")
	EItemType ItemType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Spawn(FVector localPos, EItemType type);
private:
	void MoveUpDown(float DeltaTime);

};


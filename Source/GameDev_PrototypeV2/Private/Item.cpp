// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"

#include "Math/UnitConversion.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;	
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveUpDown(DeltaTime);

}

void AItem::Spawn(FVector localPos, EItemType type)
{
	SetActorLocation(localPos);
	ItemType = type;	
}

void AItem::MoveUpDown(float DeltaTime)
{
	float sin = FMath::Sin(GetWorld()->GetTimeSeconds() * speed);
	FVector loc = FVector(GetActorLocation().X, 
						  GetActorLocation().Y,
					  GetActorLocation().Z + sin);
	SetActorLocation(loc);
}


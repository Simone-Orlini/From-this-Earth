// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "NiagaraComponent.h"
#include  "NiagaraSystem.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	VisualElementMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualElementMeshComp"));
	//VisualElementMeshComp->SetupAttachment(RootComponent);
	VisualElementMeshComp->SetVisibility(false);
	RootComponent = VisualElementMeshComp;

	//VisualElementMeshComp->SetRelativeScale3D(FVector(4.0f)); // DA ELIMINARE

	outline = CreateDefaultSubobject<UDecalComponent>(TEXT("Outline"));
	outline->SetRelativeRotation(FRotator(90.f, 0.0f, 0.0f));
	outline->SetupAttachment(RootComponent);
	//RootComponent = outline;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
	VFX->bAutoActivate = false;


	VisualElementVFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VisualElementVFXComp"));
	VisualElementVFXComp->SetupAttachment(RootComponent);
	VisualElementVFXComp->SetVisibility(false);

	coordsText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Coords"));
	coordsText->SetupAttachment(RootComponent);
	coordsText->SetRelativeLocation(FVector(0.0f, 50.0f, 0.0f));
	coordsText->SetRelativeRotation(FRotator(90.0f, 0, 0.0f));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::ApplyEffect()
{
	switch (tileData.GridTileData.TileType)
	{
	default:
	case ETileType::Bush:
	case ETileType::ToReach:
	case ETileType::Empty:
		// Do nothing
		return;
	case ETileType::Fire:
		// ApplyDamageToUnit when enter and at the start of every round
		return;
	case ETileType::Mud:
		// Half the Max movement range 5/2 = 2
		return;
	}
}

void ATile::UnitEnterTile(AActor* Actor)
{
	/* Quando una unit entra nel tile va prima definito se e' alleata o nemica
	 * In caso di alleta si deve dire al gamemode se la unit ha raccolto un item
	 * L'unita deve avere delle informazioni del tile
	 * 
	 * 
	 */


	// if (Actor == Ally)

	// The unit reached the ToReachTile
	if (tileData.GridTileData.TileType == ETileType::ToReach)
	{
		VFX->SetActive(false);
		tileData.GridTileData.TileType = ETileType::Empty;
	}
}

void ATile::SpawnTile(FTileData _tileData, TArray<UObject*> elements)
{
	InitTileData(_tileData);
	InitVisualElement(elements);

	//coordsText->SetText(FText::FromString(FString::Printf(TEXT("X: %d, Y: %d"), tileData.GridTileData.Index.X, tileData.GridTileData.Index.Y)));
	coordsText->SetText(FText::FromString(TEXT("")));

	//outline->SetStaticMesh(tileData.Outline);
	outline->SetDecalMaterial(tileData.Outline);
	outline->DecalSize = FVector(50.0f, 53.0f, 53.0f);
	VFX->SetAsset(tileData.VFX);
	VFX->SetActive(false);

	switch (tileData.GridTileData.TileType)
	{
	default:
	case ETileType::Empty:
		break;
	case ETileType::ToReach:
		VFX->SetActive(true);
		break;
	case ETileType::Bush:
		SpawnVisualElement(EMaskType::Grass);
		break;
	case ETileType::Mud:
		SpawnVisualElement(EMaskType::Mud);
		break;
	case ETileType::Fire:
		SpawnVisualElement(EMaskType::Fire);
		break;
	};

	UpdateOutline();
}

void ATile::InitTileData(FTileData _tileData)
{
	tileData = _tileData;
}

void ATile::UpdateOutline()
{
	tileData.TileStatuses.Sort([](const ETileStatus& A, const ETileStatus& B)
	{
		return (uint8)A < (uint8)B;
	});

	ETileStatus s = tileData.TileStatuses[0];

	// UE_LOG(LogTemp, Warning, TEXT("Enum:" + s));
	// if (s == ETileStatus::None)
	// {
	// 	outline->SetVisibility(false);
	// 	return;
	// }

	//outline->SetVisibility(true);

	FLinearColor colorBorder = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor colorFill = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 1.0f);
	//outline->CreateDynamicMaterialInstance()->SetScalarParameterValue(FName("Fill"), 1.0f);

	UMaterialInstanceDynamic* mat = outline->CreateDynamicMaterialInstance();
	mat->SetScalarParameterValue(FName("Fill"), 1.0f);
	outline->SetMaterial(0, mat);

	
	switch (s)
	{
	case ETileStatus::Highlight:
		colorBorder = FLinearColor(0.5f, 0.5f, 0.5f);
		colorFill = colorBorder;
		//outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 0.5f);
		//outline->CreateDynamicMaterialInstance()->SetScalarParameterValue(FName("Fill"), 0.5f);
		
		//mat = outline->CreateDynamicMaterialInstance();
		mat->SetScalarParameterValue(FName("Fill"), 0.5f);
		outline->SetMaterial(0, mat);

		break;
	case ETileStatus::Hovered:
		colorBorder = FLinearColor(0.2f, 0.35f, 0.0f);
		colorFill = colorBorder;
		break;
	case ETileStatus::Movement:
		colorBorder = FLinearColor(0.15f, 0.45f, 0.0f);
		colorFill = colorBorder;
		break;
	case ETileStatus::Attack:
		colorBorder = FLinearColor(0.9f, 0.3f, 0.0f);
		colorFill = colorBorder;
		break;
	default:
	case ETileStatus::None:
		//outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 0.0f);
		
		//mat = outline->CreateDynamicMaterialInstance();
		mat->SetScalarParameterValue(FName("Fill"), 0.0f);
		outline->SetMaterial(0, mat);
		
		break;
	}

	//outline->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName("Color_Border"), colorBorder);
	//outline->CreateDynamicMaterialInstance()->SetVectorParameterValue(FName("Fill"), colorBorder);
	//mat = outline->CreateDynamicMaterialInstance();
	mat->SetVectorParameterValue(FName("Color_Border"), colorBorder);
	outline->SetMaterial(0, mat);
	
	
	//outline->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName("Color_Fill"), colorFill);
	//outline->CreateDynamicMaterialInstance()->SetVectorParameterValue(FName("Fill"), colorFill);
	//mat = outline->CreateDynamicMaterialInstance();
	mat->SetVectorParameterValue(FName("Color_Fill"), colorFill);
	outline->SetMaterial(0, mat);
}

void ATile::AddOutlineStatus(ETileStatus newStatus)
{
	tileData.TileStatuses.AddUnique(newStatus);
}

void ATile::RemoveOutlineStatus(ETileStatus oldStatus)
{
	tileData.TileStatuses.Remove(oldStatus);
}

void ATile::EnableVFX()
{
	VFX->SetActive(true);
}

void ATile::InitVisualElement(TArray<UObject*> elements)
{
	int32 index = 0;
	for (int32 i = 0; i < elements.Num(); i++)
	{
		VisualTileTypeElements.Add(static_cast<EMaskType>(i + 1), elements[i]);
	}
}

void ATile::SpawnVisualElement(EMaskType MaskType)
{
	if (VisualTileTypeElements.Find(MaskType))
	{
		UObject** obj = VisualTileTypeElements.Find(MaskType);

		if (!*obj || !obj)
			return;


		UStaticMesh* mesh = Cast<UStaticMesh>(*obj);
		if (mesh)
		{
			VisualElementMeshComp->SetStaticMesh(mesh);
			VisualElementMeshComp->SetVisibility(true);
		}
		else
		{
			UNiagaraSystem* niagara = Cast<UNiagaraSystem>(*obj);

			if (niagara)
			{
				VisualElementVFXComp->SetAsset(niagara);
				VisualElementVFXComp->SetVisibility(true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("the visual element in this mask - %hhd - is not compatible"), MaskType)
			}
		}


		switch (MaskType)
		{
		default:
		case EMaskType::NONE:
			// Non esiste la maschera
			break;
		case EMaskType::Grass:
			tileData.GridTileData.TileType = ETileType::Bush;
			break;
		case EMaskType::Mud:
			tileData.GridTileData.TileType = ETileType::Mud;
			break;
		case EMaskType::Fire:
			tileData.GridTileData.TileType = ETileType::Fire;
			break;
		}
	}
}

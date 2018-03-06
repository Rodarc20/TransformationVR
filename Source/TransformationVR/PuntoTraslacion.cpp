// Fill out your copyright notice in the Description page of Project Settings.

#include "PuntoTraslacion.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"


// Sets default values
APuntoTraslacion::APuntoTraslacion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    Guia = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Guia"));
    Guia->SetupAttachment(RootComponent);
    Guia->SetRelativeScale3D(FVector(0.5f));
	//ParteMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> GuiaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (GuiaMeshAsset.Succeeded()) {
        Guia->SetStaticMesh(GuiaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> GuiaMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/Origen.Origen'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (GuiaMaterialAsset.Succeeded()) {
            Guia->SetMaterial(0, GuiaMaterialAsset.Object);
        }
    }

    Globo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Globo"));
	Globo->SetupAttachment(RootComponent);
	Globo->SetRelativeLocation(FVector(0.0f, 0.0f, 7.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> GloboMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (GloboMeshAsset.Succeeded()) {
        Globo->SetStaticMesh(GloboMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> GloboMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZTitilando.FlechaZTitilando'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (GloboMaterialAsset.Succeeded()) {
            Globo->SetMaterial(0, GloboMaterialAsset.Object);
        }
    }

}

// Called when the game starts or when spawned
void APuntoTraslacion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuntoTraslacion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


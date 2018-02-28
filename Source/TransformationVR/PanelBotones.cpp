// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelBotones.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"


// Sets default values
APanelBotones::APanelBotones()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Centro"));

    FVector EscalaBotones = FVector(0.5f);
	BotonTraslacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonTraslacion"));
    BotonTraslacion->SetupAttachment(RootComponent);
	//BotonTraslacion->SetRelativeLocation(FVector(10.0f, -15.0f, 0.0f));
	BotonTraslacion->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	BotonTraslacion->SetRelativeScale3D(EscalaBotones);

	BotonRotacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonRotacion"));
    BotonRotacion->SetupAttachment(RootComponent);
	//BotonRotacion->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	BotonRotacion->SetRelativeLocation(FVector(0.0f, -25.0f, 0.0f));
	BotonRotacion->SetRelativeScale3D(EscalaBotones);

	BotonEscala = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonEscala"));
    BotonEscala->SetupAttachment(RootComponent);
	//BotonEscala->SetRelativeLocation(FVector(10.0f, 15.0f, 0.0f));
	BotonEscala->SetRelativeLocation(FVector(0.0f, -10.0f, 0.0f));
	BotonEscala->SetRelativeScale3D(EscalaBotones);

	BotonX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonX"));
    BotonX->SetupAttachment(RootComponent);
	//BotonX->SetRelativeLocation(FVector(-10.0f, -15.0f, 0.0f));
	BotonX->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
	BotonX->SetRelativeScale3D(EscalaBotones);

	BotonY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonY"));
    BotonY->SetupAttachment(RootComponent);
	//BotonY->SetRelativeLocation(FVector(-10.0f, 0.0f, 0.0f));
	BotonY->SetRelativeLocation(FVector(0.0f, 25.0f, 0.0f));
	BotonY->SetRelativeScale3D(EscalaBotones);
	//BotonRotacion->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));

	BotonZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonZ"));
    BotonZ->SetupAttachment(RootComponent);
	//BotonZ->SetRelativeLocation(FVector(-10.0f, 15.0f, 0.0f));
	BotonZ->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	BotonZ->SetRelativeScale3D(EscalaBotones);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BotonAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_Boton.BotonS_Boton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BotonAsset.Succeeded()) {
        BotonTraslacion->SetStaticMesh(BotonAsset.Object);
        BotonRotacion->SetStaticMesh(BotonAsset.Object);
        BotonEscala->SetStaticMesh(BotonAsset.Object);
        BotonX->SetStaticMesh(BotonAsset.Object);
        BotonY->SetStaticMesh(BotonAsset.Object);
        BotonZ->SetStaticMesh(BotonAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonTMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_5.BotonMaterial_5'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonTMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;//quiza necesite conservar materiales par poder hacer los efectos
            BotonTraslacion->SetMaterial(0, BotonTMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonRMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_6.BotonMaterial_6'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonRMaterialAsset.Succeeded()) {
			//EjeYNormal = FlechaYMaterialAsset.Object;
            BotonRotacion->SetMaterial(0, BotonRMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonEMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_7.BotonMaterial_7'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonEMaterialAsset.Succeeded()) {
			//EjeZNormal = FlechaZMaterialAsset.Object;
            BotonEscala->SetMaterial(0, BotonEMaterialAsset.Object);
        }

        static ConstructorHelpers::FObjectFinder<UMaterial> BotonXMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_4.BotonMaterial_4'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonXMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;//quiza necesite conservar materiales par poder hacer los efectos
            BotonX->SetMaterial(0, BotonXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonYMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_2.BotonMaterial_2'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonYMaterialAsset.Succeeded()) {
			//EjeYNormal = FlechaYMaterialAsset.Object;
            BotonY->SetMaterial(0, BotonYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonZMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_3.BotonMaterial_3'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonZMaterialAsset.Succeeded()) {
			//EjeZNormal = FlechaZMaterialAsset.Object;
            BotonZ->SetMaterial(0, BotonZMaterialAsset.Object);
        }
    }

	BordeTraslacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeTraslacion"));
    BordeTraslacion->SetupAttachment(RootComponent);
	//BordeTraslacion->SetRelativeLocation(FVector(10.0f, -15.0f, 0.0f));
	BordeTraslacion->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	BordeTraslacion->SetRelativeScale3D(EscalaBotones);

	BordeRotacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeRotacion"));
    BordeRotacion->SetupAttachment(RootComponent);
	//BordeRotacion->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	BordeRotacion->SetRelativeLocation(FVector(0.0f, -25.0f, 0.0f));
	BordeRotacion->SetRelativeScale3D(EscalaBotones);

	BordeEscala = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeEscala"));
    BordeEscala->SetupAttachment(RootComponent);
	//BordeEscala->SetRelativeLocation(FVector(10.0f, 15.0f, 0.0f));
	BordeEscala->SetRelativeLocation(FVector(0.0f, -10.0f, 0.0f));
	BordeEscala->SetRelativeScale3D(EscalaBotones);

	BordeX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeX"));
    BordeX->SetupAttachment(RootComponent);
	//BordeX->SetRelativeLocation(FVector(-10.0f, -15.0f, 0.0f));
	BordeX->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
	BordeX->SetRelativeScale3D(EscalaBotones);

	BordeY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeY"));
    BordeY->SetupAttachment(RootComponent);
	//BordeY->SetRelativeLocation(FVector(-10.0f, 0.0f, 0.0f));
	BordeY->SetRelativeLocation(FVector(0.0f, 25.0f, 0.0f));
	BordeY->SetRelativeScale3D(EscalaBotones);

	BordeZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeZ"));
    BordeZ->SetupAttachment(RootComponent);
	//BordeZ->SetRelativeLocation(FVector(-10.0f, 15.0f, 0.0f));
	BordeZ->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	BordeZ->SetRelativeScale3D(EscalaBotones);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BordeAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_BordeBoton.BotonS_BordeBoton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BordeAsset.Succeeded()) {
        BordeTraslacion->SetStaticMesh(BordeAsset.Object);
        BordeRotacion->SetStaticMesh(BordeAsset.Object);
        BordeEscala->SetStaticMesh(BordeAsset.Object);
        BordeX->SetStaticMesh(BordeAsset.Object);
        BordeY->SetStaticMesh(BordeAsset.Object);
        BordeZ->SetStaticMesh(BordeAsset.Object);
        //sera el mismoborde para todos los botones, por lo tanto es el mismo material para todos
        static ConstructorHelpers::FObjectFinder<UMaterial> BordeMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BordeMaterial.BordeMaterial'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BordeMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;//quiza necesite conservar materiales par poder hacer los efectos
            BordeTraslacion->SetMaterial(0, BordeMaterialAsset.Object);
            BordeRotacion->SetMaterial(0, BordeMaterialAsset.Object);
            BordeEscala->SetMaterial(0, BordeMaterialAsset.Object);
            BordeX->SetMaterial(0, BordeMaterialAsset.Object);
            BordeY->SetMaterial(0, BordeMaterialAsset.Object);
            BordeZ->SetMaterial(0, BordeMaterialAsset.Object);
        }
    }
}

// Called when the game starts or when spawned
void APanelBotones::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APanelBotones::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


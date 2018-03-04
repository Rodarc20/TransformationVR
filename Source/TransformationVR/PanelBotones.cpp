// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelBotones.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Casa.h"


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
	BotonTraslacion->SetRelativeLocation(FVector(0.0f, -15.0f, 0.0f));
	BotonTraslacion->SetRelativeScale3D(EscalaBotones);

	BotonRotacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonRotacion"));
    BotonRotacion->SetupAttachment(RootComponent);
	//BotonRotacion->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	BotonRotacion->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BotonRotacion->SetRelativeScale3D(EscalaBotones);

	BotonEscala = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotonEscala"));
    BotonEscala->SetupAttachment(RootComponent);
	//BotonEscala->SetRelativeLocation(FVector(10.0f, 15.0f, 0.0f));
	BotonEscala->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	BotonEscala->SetRelativeScale3D(EscalaBotones);


    static ConstructorHelpers::FObjectFinder<UStaticMesh> BotonAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_Boton.BotonS_Boton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BotonAsset.Succeeded()) {
        BotonTraslacion->SetStaticMesh(BotonAsset.Object);
        BotonRotacion->SetStaticMesh(BotonAsset.Object);
        BotonEscala->SetStaticMesh(BotonAsset.Object);
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
    }

	BordeTraslacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeTraslacion"));
    BordeTraslacion->SetupAttachment(RootComponent);
	//BordeTraslacion->SetRelativeLocation(FVector(10.0f, -15.0f, 0.0f));
	BordeTraslacion->SetRelativeLocation(FVector(0.0f, -15.0f, 0.0f));
	BordeTraslacion->SetRelativeScale3D(EscalaBotones);

	BordeRotacion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeRotacion"));
    BordeRotacion->SetupAttachment(RootComponent);
	//BordeRotacion->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	BordeRotacion->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BordeRotacion->SetRelativeScale3D(EscalaBotones);

	BordeEscala = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BordeEscala"));
    BordeEscala->SetupAttachment(RootComponent);
	//BordeEscala->SetRelativeLocation(FVector(10.0f, 15.0f, 0.0f));
	BordeEscala->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	BordeEscala->SetRelativeScale3D(EscalaBotones);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BordeAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_BordeBoton.BotonS_BordeBoton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BordeAsset.Succeeded()) {
        BordeTraslacion->SetStaticMesh(BordeAsset.Object);
        BordeRotacion->SetStaticMesh(BordeAsset.Object);
        BordeEscala->SetStaticMesh(BordeAsset.Object);
        //sera el mismoborde para todos los botones, por lo tanto es el mismo material para todos
        static ConstructorHelpers::FObjectFinder<UMaterial> BordeMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BordeMaterial.BordeMaterial'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BordeMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;//quiza necesite conservar materiales par poder hacer los efectos
            BordeTraslacion->SetMaterial(0, BordeMaterialAsset.Object);
            BordeRotacion->SetMaterial(0, BordeMaterialAsset.Object);
            BordeEscala->SetMaterial(0, BordeMaterialAsset.Object);
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

void APanelBotones::Press(ETransformacionTarea BotonPresionado) {
    //quien tiene a este boton? puedo poner de nuevo verificar quien es el owner, y castear y llamar a su funcion correspondiente, probar con Casa primero
    ACasa * Casa = Cast<ACasa>(GetOwner());
    if (Casa) {
        Casa->SetTransformacionTarea(BotonPresionado);
    }
}

void APanelBotones::Release(ETransformacionTarea BotonPresionado) {
}


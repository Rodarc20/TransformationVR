// Fill out your copyright notice in the Description page of Project Settings.

#include "BotonTrasladar.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"

ABotonTrasladar::ABotonTrasladar() {
	PrimaryActorTick.bCanEverTick = true;

    FVector EscalaBotones = FVector(0.5f);
    
	Borde = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Borde"));
    RootComponent = Borde;
    //Borde->SetupAttachment(RootComponent);
	Borde->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Borde->SetWorldScale3D(EscalaBotones);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BordeAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_BordeBoton.BotonS_BordeBoton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BordeAsset.Succeeded()) {
        Borde->SetStaticMesh(BordeAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> BordeMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BordeMaterial.BordeMaterial'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BordeMaterialAsset.Succeeded()) {
            Borde->SetMaterial(0, BordeMaterialAsset.Object);
        }
    }

	Boton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boton"));
    Boton->SetupAttachment(RootComponent);
	Boton->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Boton->SetWorldScale3D(EscalaBotones);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BotonAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Botones/BotonS_Boton.BotonS_Boton'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BotonAsset.Succeeded()) {
        Boton->SetStaticMesh(BotonAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> BotonMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Botones/BotonMaterial_5.BotonMaterial_5'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (BotonMaterialAsset.Succeeded()) {
            Boton->SetMaterial(0, BotonMaterialAsset.Object);
        }
    }

    VelocidadNormal = 1.0f;
}

void ABotonTrasladar::BeginPlay() {
    Super::BeginPlay();
    Boton->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
}

void ABotonTrasladar::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (!bPosicionNormal) {
        Boton->SetRelativeLocation(FMath::Lerp(Boton->GetRelativeTransform().GetLocation(), FVector::ZeroVector, DeltaTime * VelocidadNormal));
        if (Boton->GetRelativeTransform().GetLocation() == FVector::ZeroVector) {
            bPosicionNormal = true;
        }
    }
    if (bPressing) {//osea hay un elemento entrando en contacto con el boton
    }
  
}

//necesito agregar la colision para el boton, con el cual ira manteniendo la distancia con el control, hasta que no pase de los limites, es decir posicino de descanso y posicion fondo
//
//no debo exceder esos limitas asi podre hacer que salga de la colision
//igula si el control baja en exceso
// Fill out your copyright notice in the Description page of Project Settings.

#include "Cabeza.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

ACabeza::ACabeza() {

    Cabeza = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabeza"));
	RootComponent = Cabeza;
	Cabeza->SetWorldScale3D(FVector(0.1f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CabezaMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (CabezaMeshAsset.Succeeded()) {
        Cabeza->SetStaticMesh(CabezaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CabezaMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (CabezaMaterialAsset.Succeeded()) {
            Cabeza->SetMaterial(0, CabezaMaterialAsset.Object);
        }
    }

	ArticulacionCuello = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCuello"));
    ArticulacionCuello->SetupAttachment(RootComponent);
	ArticulacionCuello->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
	ArticulacionCuello->SetWorldScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (CuelloMeshAsset.Succeeded()) {
        ArticulacionCuello->SetStaticMesh(CuelloMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CuelloMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (CuelloMaterialAsset.Succeeded()) {
            ArticulacionCuello->SetMaterial(0, CuelloMaterialAsset.Object);
        }
    }
}


void ACabeza::BeginPlay() {

}


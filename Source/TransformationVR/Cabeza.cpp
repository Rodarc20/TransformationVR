// Fill out your copyright notice in the Description page of Project Settings.

#include "Cabeza.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "TransformacionWidget.h"

ACabeza::ACabeza() {

	Id = 1;
	IdParteRaiz = Id;
	NombreParte = "Cabeza";

	ColisionCuello = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionCuello"));
	RootComponent = ColisionCuello;
    ColisionCuello->InitSphereRadius(2.0f);
	ColisionCuello->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCuello->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCuello = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCuello"));
    ArticulacionCuello->SetupAttachment(RootComponent);
	//ArticulacionCuello->SetWorldScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Partes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (CuelloMeshAsset.Succeeded()) {
        ArticulacionCuello->SetStaticMesh(CuelloMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CuelloMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (CuelloMaterialAsset.Succeeded()) {
            ArticulacionCuello->SetMaterial(0, CuelloMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }


    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
	ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CabezaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Partes/Cabeza.Cabeza'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (CabezaMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(CabezaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CabezaMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (CabezaMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, CabezaMaterialAsset.Object);
        }
    }
    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;


	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(0.0f, 0.0f, 4.0f));
    Colision->InitCapsuleSize(3.5f, 6.5f);


	MeshesArticulaciones.Add(ArticulacionCuello);
	ColisionesArticualciones.Add(ColisionCuello);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);

	ColorArticulacionNoConectada = UKismetMathLibrary::HSVToRGB(231.0f, 1.0f, 1.0f, 1.0f);

	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(218.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(218.0f);

}

void ACabeza::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCuello->GetMaterial(0), this));
	ArticulacionCuello->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(0);
	//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(ArticulacionCuello->GetMaterial(0), this);
	//ArticulacionCuello->SetMaterial(0, ArticulacionMaterialDynamic);
}







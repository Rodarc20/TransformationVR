// Fill out your copyright notice in the Description page of Project Settings.

#include "PiernaIzquierda.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

APiernaIzquierda::APiernaIzquierda() {

	Id = 5;
	IdParteRaiz = Id;
	NombreParte = "Pierna Izquierda";

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionCaderaI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionCaderaI"));
	RootComponent = ColisionCaderaI;
    ColisionCaderaI->InitSphereRadius(2.0f);
	ColisionCaderaI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCaderaI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaI"));
    ArticulacionCaderaI->SetupAttachment(RootComponent);
	ArticulacionCaderaI->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionCaderaI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionCaderaI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }


	ArticulacionTobilloI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionTobilloI"));
    ArticulacionTobilloI->SetupAttachment(RootComponent);
	ArticulacionTobilloI->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionTobilloI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionTobilloI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionTobilloI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionTobilloI"));
	ColisionTobilloI->SetupAttachment(RootComponent);
	ColisionTobilloI->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));
    ColisionTobilloI->InitSphereRadius(2.0f);
	ColisionTobilloI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionTobilloI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
    ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	ParteMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PiernaIMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/PiernaI.PiernaI'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PiernaIMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(PiernaIMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.25f, 12.0f);


	MeshesArticulaciones.Add(ArticulacionCaderaI);
	ColisionesArticualciones.Add(ColisionCaderaI);
	MeshesArticulaciones.Add(ArticulacionTobilloI);
	ColisionesArticualciones.Add(ColisionTobilloI);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void APiernaIzquierda::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCaderaI->GetMaterial(0), this));
	ArticulacionCaderaI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionTobilloI->GetMaterial(0), this));
	ArticulacionTobilloI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}





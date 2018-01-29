// Fill out your copyright notice in the Description page of Project Settings.

#include "PieDerecho.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

APieDerecho::APieDerecho() {

	Id = 8;
	IdParteRaiz = Id;
	NombreParte = "Pie Derecho";

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionTobilloD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionTobilloD"));
	RootComponent = ColisionTobilloD;
    ColisionTobilloD->InitSphereRadius(2.0f);
	ColisionTobilloD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionTobilloD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionTobilloD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionTobilloD"));
    ArticulacionTobilloD->SetupAttachment(RootComponent);
	ArticulacionTobilloD->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionTobilloD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionTobilloD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
	ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.5f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PieDMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/PieD.PieD'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PieDMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(PieDMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }
    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(2.119f, 0.0f, -5.5f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(5.0f, 5.25f);

	MeshesArticulaciones.Add(ArticulacionTobilloD);
	ColisionesArticualciones.Add(ColisionTobilloD);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void APieDerecho::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionTobilloD->GetMaterial(0), this));
	ArticulacionTobilloD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}





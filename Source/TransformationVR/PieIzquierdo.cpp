// Fill out your copyright notice in the Description page of Project Settings.

#include "PieIzquierdo.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

APieIzquierdo::APieIzquierdo() {

	Id = 9;
	IdParteRaiz = Id;
	NombreParte = "Pie Izquierdo";

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionTobilloI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionTobilloI"));
	RootComponent = ColisionTobilloI;
    ColisionTobilloI->InitSphereRadius(2.0f);
	ColisionTobilloI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionTobilloI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionTobilloI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionTobilloI"));
    ArticulacionTobilloI->SetupAttachment(RootComponent);
	ArticulacionTobilloI->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionTobilloI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionTobilloI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
    ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.5f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PieIMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/PieI.PieI'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PieIMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(PieIMeshAsset.Object);
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


	MeshesArticulaciones.Add(ArticulacionTobilloI);
	ColisionesArticualciones.Add(ColisionTobilloI);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(178.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(178.0f);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void APieIzquierdo::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionTobilloI->GetMaterial(0), this));
	ArticulacionTobilloI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(0);

}

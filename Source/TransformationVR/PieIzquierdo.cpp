// Fill out your copyright notice in the Description page of Project Settings.

#include "PieIzquierdo.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

APieIzquierdo::APieIzquierdo() {
    PieI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieIzquierdo"));
    //Torso->SetupAttachment(RootComponent);
	RootComponent = PieI;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PieIMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/PieI.PieI'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PieIMeshAsset.Succeeded()) {
        PieI->SetStaticMesh(PieIMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            PieI->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(2.119f, 0.0f, 0.0f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(5.0f, 5.25f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ArticulacionTobilloI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionTobilloI"));
    ArticulacionTobilloI->SetupAttachment(RootComponent);
	ArticulacionTobilloI->SetRelativeLocation(FVector(0.0f, 0.0f, 5.5f));

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
	ColisionTobilloI->SetRelativeLocation(FVector(0.0f, 0.0f, 5.5f));
    ColisionTobilloI->InitSphereRadius(2.0f);
	ColisionTobilloI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionTobilloI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	MeshesArticulaciones.Add(ArticulacionTobilloI);
	ColisionesArticualciones.Add(ColisionTobilloI);
}

void APieIzquierdo::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionTobilloI->GetMaterial(0), this));
	ArticulacionTobilloI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}

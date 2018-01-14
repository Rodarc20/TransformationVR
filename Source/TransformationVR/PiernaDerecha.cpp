// Fill out your copyright notice in the Description page of Project Settings.

#include "PiernaDerecha.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

APiernaDerecha::APiernaDerecha() {

	Id = 4;
	NombreParte = "Pierna Derecha";
    PiernaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PiernaDerecha"));
    //Torso->SetupAttachment(RootComponent);
	RootComponent = PiernaD;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PiernaDMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/PiernaD.PiernaD'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PiernaDMeshAsset.Succeeded()) {
        PiernaD->SetStaticMesh(PiernaDMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            PiernaD->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.25f, 12.0f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera


	ArticulacionCaderaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaD"));
    ArticulacionCaderaD->SetupAttachment(RootComponent);
	ArticulacionCaderaD->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionCaderaD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionCaderaD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionCaderaD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionCaderaD"));
	ColisionCaderaD->SetupAttachment(RootComponent);
	ColisionCaderaD->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
    ColisionCaderaD->InitSphereRadius(2.0f);
	ColisionCaderaD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionTobilloD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionTobilloD"));
    ArticulacionTobilloD->SetupAttachment(RootComponent);
	ArticulacionTobilloD->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionTobilloD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionTobilloD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionTobilloD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionTobilloD"));
	ColisionTobilloD->SetupAttachment(RootComponent);
	ColisionTobilloD->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
    ColisionTobilloD->InitSphereRadius(2.0f);
	ColisionTobilloD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionTobilloD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	MeshesArticulaciones.Add(ArticulacionCaderaD);
	ColisionesArticualciones.Add(ColisionCaderaD);
	MeshesArticulaciones.Add(ArticulacionTobilloD);
	ColisionesArticualciones.Add(ColisionTobilloD);
}

void APiernaDerecha::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCaderaD->GetMaterial(0), this));
	ArticulacionCaderaD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionTobilloD->GetMaterial(0), this));
	ArticulacionTobilloD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}








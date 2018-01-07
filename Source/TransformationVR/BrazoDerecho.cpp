// Fill out your copyright notice in the Description page of Project Settings.

#include "BrazoDerecho.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

ABrazoDerecho::ABrazoDerecho() {
    BrazoD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrazoDerecho"));
    //Torso->SetupAttachment(RootComponent);
	RootComponent = BrazoD;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BrazoDMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/BrazoD.BrazoD'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BrazoDMeshAsset.Succeeded()) {
        BrazoD->SetStaticMesh(BrazoDMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            BrazoD->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.0f, 10.0f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera


	ArticulacionHombroD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroD"));
    ArticulacionHombroD->SetupAttachment(RootComponent);
	ArticulacionHombroD->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionHombroD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionHombroD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionHombroD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionHombroD"));
	ColisionHombroD->SetupAttachment(RootComponent);
	ColisionHombroD->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));
    ColisionHombroD->InitSphereRadius(2.0f);
	ColisionHombroD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionMunecaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionMunecaD"));
    ArticulacionMunecaD->SetupAttachment(RootComponent);
	ArticulacionMunecaD->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionMunecaD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionMunecaD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionMunecaD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionMunecaD"));
	ColisionMunecaD->SetupAttachment(RootComponent);
	ColisionMunecaD->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));
    ColisionMunecaD->InitSphereRadius(2.0f);
	ColisionMunecaD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionMunecaD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	MeshesArticulaciones.Add(ArticulacionHombroD);
	ColisionesArticualciones.Add(ColisionHombroD);
	MeshesArticulaciones.Add(ArticulacionMunecaD);
	ColisionesArticualciones.Add(ColisionMunecaD);
}

void ABrazoDerecho::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionHombroD->GetMaterial(0), this));
	ArticulacionHombroD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionMunecaD->GetMaterial(0), this));
	ArticulacionMunecaD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}





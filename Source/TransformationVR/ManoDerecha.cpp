// Fill out your copyright notice in the Description page of Project Settings.

#include "ManoDerecha.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

AManoDerecha::AManoDerecha() {

	Id = 6;
	NombreParte = "Mano Derecha";

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionMunecaD = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionMunecaD"));
	RootComponent = ColisionMunecaD;
    ColisionMunecaD->InitSphereRadius(2.0f);
	ColisionMunecaD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionMunecaD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionMunecaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionMunecaD"));
    ArticulacionMunecaD->SetupAttachment(RootComponent);
	ArticulacionMunecaD->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionMunecaD->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionMunecaD->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }


    ManoD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ManoDerecha"));
	ManoD->SetupAttachment(RootComponent);
	ManoD->SetRelativeLocation(FVector(5.5f, 0.0f, 0.0f));
	ManoD->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ManoDMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/ManoD.ManoD'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ManoDMeshAsset.Succeeded()) {
        ManoD->SetStaticMesh(ManoDMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            ManoD->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(2.25f, 0.0f, 0.0f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.8f, 5.25f);

	MeshesArticulaciones.Add(ArticulacionMunecaD);
	ColisionesArticualciones.Add(ColisionMunecaD);
}

void AManoDerecha::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionMunecaD->GetMaterial(0), this));
	ArticulacionMunecaD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}





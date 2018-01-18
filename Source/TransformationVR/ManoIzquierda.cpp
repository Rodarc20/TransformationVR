// Fill out your copyright notice in the Description page of Project Settings.

#include "ManoIzquierda.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

AManoIzquierda::AManoIzquierda() {

	Id = 7;
	NombreParte = "Mano Izquierda";

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionMunecaI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionMunecaI"));
	RootComponent = ColisionMunecaI;
    ColisionMunecaI->InitSphereRadius(2.0f);
	ColisionMunecaI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionMunecaI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionMunecaI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionMunecaI"));
    ArticulacionMunecaI->SetupAttachment(RootComponent);
	ArticulacionMunecaI->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionMunecaI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionMunecaI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }


    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
	ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(5.5f, 0.0f, 0.0f));
	ParteMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ManoIMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/ManoI.ManoI'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ManoIMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(ManoIMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }
    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(2.25f, 0.0f, 0.0f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.8f, 5.25f);


	MeshesArticulaciones.Add(ArticulacionMunecaI);
	ColisionesArticualciones.Add(ColisionMunecaI);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void AManoIzquierda::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionMunecaI->GetMaterial(0), this));
	ArticulacionMunecaI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);

}





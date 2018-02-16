// Fill out your copyright notice in the Description page of Project Settings.

#include "BrazoIzquierdo.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

ABrazoIzquierdo::ABrazoIzquierdo() {

	Id = 3;
	IdParteRaiz = Id;
	NombreParte = "Brazo Izquierdo";


    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera

	ColisionHombroI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionHombroI"));
	RootComponent = ColisionHombroI;
	//ColisionHombroI->SetupAttachment(RootComponent);
    ColisionHombroI->InitSphereRadius(2.0f);
	ColisionHombroI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionHombroI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroI"));
    ArticulacionHombroI->SetupAttachment(RootComponent);
	ArticulacionHombroI->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionHombroI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionHombroI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }


	ArticulacionMunecaI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionMunecaI"));
    ArticulacionMunecaI->SetupAttachment(RootComponent);
	ArticulacionMunecaI->SetRelativeLocation(FVector(16.0f, 0.0f, 0.0f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionMunecaI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionMunecaI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionMunecaI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionMunecaI"));
	ColisionMunecaI->SetupAttachment(RootComponent);
	ColisionMunecaI->SetRelativeLocation(FVector(16.0f, 0.0f, 0.0f));
    ColisionMunecaI->InitSphereRadius(2.0f);
	ColisionMunecaI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionMunecaI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
	ParteMesh->SetupAttachment(RootComponent);
	ParteMesh->SetRelativeLocation(FVector(8.0f, 0.0f, 0.0f));
	ParteMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BrazoIMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/BrazoI.BrazoI'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (BrazoIMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(BrazoIMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ParteMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ParteMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, ParteMaterialAsset.Object);
        }
    }

    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(8.0f, 0.0f, 0.0f));
	Colision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(2.0f, 10.0f);

	MeshesArticulaciones.Add(ArticulacionHombroI);
	ColisionesArticualciones.Add(ColisionHombroI);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(298.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(298.0f);

	MeshesArticulaciones.Add(ArticulacionMunecaI);
	ColisionesArticualciones.Add(ColisionMunecaI);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(98.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(98.0f);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void ABrazoIzquierdo::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionHombroI->GetMaterial(0), this));
	ArticulacionHombroI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(0);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionMunecaI->GetMaterial(0), this));
	ArticulacionMunecaI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(1);

}

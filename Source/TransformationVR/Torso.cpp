// Fill out your copyright notice in the Description page of Project Settings.

#include "Torso.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

ATorso::ATorso() {

    Torso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torso"));
    //Torso->SetupAttachment(RootComponent);
	RootComponent = Torso;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> TorsoMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Torso.Torso'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (TorsoMeshAsset.Succeeded()) {
        Torso->SetStaticMesh(TorsoMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> TorsoMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (TorsoMaterialAsset.Succeeded()) {
            Torso->SetMaterial(0, TorsoMaterialAsset.Object);
        }
    }

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(5.0f, 8.0f);

	ArticulacionCuello = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCuello"));
    ArticulacionCuello->SetupAttachment(RootComponent);
	ArticulacionCuello->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	ArticulacionCuello->SetWorldScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	static ConstructorHelpers::FObjectFinder<UMaterial> ArticulacionMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/BurbujaArticulacion.BurbujaArticulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionCuello->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionCuello->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionCuello = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionCuello"));
	ColisionCuello->SetupAttachment(RootComponent);
	ColisionCuello->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
    ColisionCuello->InitSphereRadius(2.5f);
	ColisionCuello->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCuello->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionHombroD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroD"));
    ArticulacionHombroD->SetupAttachment(RootComponent);
	ArticulacionHombroD->SetRelativeLocation(FVector(0.0f, 11.0f, 7.5f));
	ArticulacionHombroD->SetWorldScale3D(FVector(0.05f));

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
	ColisionHombroD->SetRelativeLocation(FVector(0.0f, 11.0f, 7.5f));
    ColisionHombroD->InitSphereRadius(2.5f);
	ColisionHombroD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCaderaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaD"));
    ArticulacionCaderaD->SetupAttachment(RootComponent);
	ArticulacionCaderaD->SetRelativeLocation(FVector(0.0f, 8.5f, -10.0f));
	ArticulacionCaderaD->SetWorldScale3D(FVector(0.05f));

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
	ColisionCaderaD->SetRelativeLocation(FVector(0.0f, 8.5f, -10.0f));
    ColisionCaderaD->InitSphereRadius(2.5f);
	ColisionCaderaD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionHombroI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroI"));
    ArticulacionHombroI->SetupAttachment(RootComponent);
	ArticulacionHombroI->SetRelativeLocation(FVector(0.0f, -11.0f, 7.5f));
	ArticulacionHombroI->SetWorldScale3D(FVector(0.05f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionHombroI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionHombroI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionHombroI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionHombroI"));
	ColisionHombroI->SetupAttachment(RootComponent);
	ColisionHombroI->SetRelativeLocation(FVector(0.0f, -11.0f, 7.5f));
    ColisionHombroI->InitSphereRadius(2.5f);
	ColisionHombroI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCaderaI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaI"));
    ArticulacionCaderaI->SetupAttachment(RootComponent);
	ArticulacionCaderaI->SetRelativeLocation(FVector(0.0f, -8.5f, -10.0f));
	ArticulacionCaderaI->SetWorldScale3D(FVector(0.05f));

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CuelloMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArticulacionMeshAsset.Succeeded()) {
        ArticulacionCaderaI->SetStaticMesh(ArticulacionMeshAsset.Object);
        if (ArticulacionMaterialAsset.Succeeded()) {
            ArticulacionCaderaI->SetMaterial(0, ArticulacionMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	ColisionCaderaI = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionCaderaI"));
	ColisionCaderaI->SetupAttachment(RootComponent);
	ColisionCaderaI->SetRelativeLocation(FVector(0.0f, -8.5f, -10.0f));
    ColisionCaderaI->InitSphereRadius(2.5f);
	ColisionCaderaI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	MeshesArticulaciones.Add(ArticulacionCuello);
	ColisionesArticualciones.Add(ColisionCuello);
	MeshesArticulaciones.Add(ArticulacionHombroD);
	ColisionesArticualciones.Add(ColisionHombroD);
	MeshesArticulaciones.Add(ArticulacionCaderaD);
	ColisionesArticualciones.Add(ColisionCaderaD);
	MeshesArticulaciones.Add(ArticulacionHombroI);
	ColisionesArticualciones.Add(ColisionHombroI);
	MeshesArticulaciones.Add(ArticulacionCaderaI);
	ColisionesArticualciones.Add(ColisionCaderaI);

}

void ATorso::BeginPlay() {

}
void ATorso::CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor) {
}

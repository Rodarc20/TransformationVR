// Fill out your copyright notice in the Description page of Project Settings.

#include "Torso.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

ATorso::ATorso() {

	PrimaryActorTick.bCanEverTick = true;

	Id = 0;
	IdParteRaiz = Id;
	NombreParte = "Torso";
    ParteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParteMesh"));
    //Torso->SetupAttachment(RootComponent);
	RootComponent = ParteMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> TorsoMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Partes/Torso.Torso'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (TorsoMeshAsset.Succeeded()) {
        ParteMesh->SetStaticMesh(TorsoMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> TorsoMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/ParteBasico.ParteBasico'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (TorsoMaterialAsset.Succeeded()) {
            ParteMesh->SetMaterial(0, TorsoMaterialAsset.Object);
        }
    }
    ParteMesh->SetCollisionProfileName(FName(TEXT("Parte")));
	ParteMesh->bRenderCustomDepth = true;

	Colision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Colision"));
	Colision->SetupAttachment(RootComponent);
	Colision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Colision->InitCapsuleSize(8.0f, 13.0f);

	ArticulacionCuello = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCuello"));
    ArticulacionCuello->SetupAttachment(RootComponent);
	ArticulacionCuello->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArticulacionMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Partes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
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
    ColisionCuello->InitSphereRadius(2.0f);
	ColisionCuello->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCuello->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionHombroD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroD"));
    ArticulacionHombroD->SetupAttachment(RootComponent);
	ArticulacionHombroD->SetRelativeLocation(FVector(0.0f, 11.0f, 7.5f));

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
    ColisionHombroD->InitSphereRadius(2.0f);
	ColisionHombroD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCaderaD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaD"));
    ArticulacionCaderaD->SetupAttachment(RootComponent);
	ArticulacionCaderaD->SetRelativeLocation(FVector(0.0f, 6.5f, -10.0f));

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
	ColisionCaderaD->SetRelativeLocation(FVector(0.0f, 6.5f, -10.0f));
    ColisionCaderaD->InitSphereRadius(2.0f);
	ColisionCaderaD->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaD->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionHombroI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionHombroI"));
    ArticulacionHombroI->SetupAttachment(RootComponent);
	ArticulacionHombroI->SetRelativeLocation(FVector(0.0f, -11.0f, 7.5f));

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
    ColisionHombroI->InitSphereRadius(2.0f);
	ColisionHombroI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionHombroI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	ArticulacionCaderaI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArticulacionCaderaI"));
    ArticulacionCaderaI->SetupAttachment(RootComponent);
	ArticulacionCaderaI->SetRelativeLocation(FVector(0.0f, -6.5f, -10.0f));

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
	ColisionCaderaI->SetRelativeLocation(FVector(0.0f, -6.5f, -10.0f));
    ColisionCaderaI->InitSphereRadius(2.0f);
	ColisionCaderaI->OnComponentBeginOverlap.AddDynamic(this, &AParte::OnBeginOverlapArticulacion);
	ColisionCaderaI->OnComponentEndOverlap.AddDynamic(this, &AParte::OnEndOverlapArticulacion);

	MeshesArticulaciones.Add(ArticulacionCuello);
	ColisionesArticualciones.Add(ColisionCuello);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(218.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(218.0f);
	
	MeshesArticulaciones.Add(ArticulacionHombroD);
	ColisionesArticualciones.Add(ColisionHombroD);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(258.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(258.0f);

	MeshesArticulaciones.Add(ArticulacionCaderaD);
	ColisionesArticualciones.Add(ColisionCaderaD);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(338.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(338.0f);

	MeshesArticulaciones.Add(ArticulacionHombroI);
	ColisionesArticualciones.Add(ColisionHombroI);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(298.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(298.0f);

	MeshesArticulaciones.Add(ArticulacionCaderaI);
	ColisionesArticualciones.Add(ColisionCaderaI);
	ColoresArticulaciones.Add(UKismetMathLibrary::HSVToRGB(18.0f, 1.0f, 1.0f, 1.0f));
	HueArticulaciones.Add(18.0f);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void ATorso::BeginPlay() {
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCuello->GetMaterial(0), this));
	ArticulacionCuello->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(0);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionHombroD->GetMaterial(0), this));
	ArticulacionHombroD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(1);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCaderaD->GetMaterial(0), this));
	ArticulacionCaderaD->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(2);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionHombroI->GetMaterial(0), this));
	ArticulacionHombroI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(3);
	ArticulacionesMaterialDynamic.Add(UMaterialInstanceDynamic::Create(ArticulacionCaderaI->GetMaterial(0), this));
	ArticulacionCaderaI->SetMaterial(0, ArticulacionesMaterialDynamic[ArticulacionesMaterialDynamic.Num()-1]);
	ColorNormalArticulacion(4);

}

void ATorso::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UE_LOG(LogClass, Log, TEXT("Torso"));

}


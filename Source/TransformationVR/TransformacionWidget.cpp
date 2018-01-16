// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformacionWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

// Sets default values for this component's properties
UTransformacionWidget::UTransformacionWidget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Origen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Origen"));
    Origen->SetupAttachment(this);
	Origen->SetRelativeScale3D(FVector(0.40f));
	//ArticulacionCuello->SetWorldScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> OrigenAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (OrigenAsset.Succeeded()) {
        Origen->SetStaticMesh(OrigenAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> OrigenMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/Origen.Origen'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (OrigenMaterialAsset.Succeeded()) {
            Origen->SetMaterial(0, OrigenMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	FlechaX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaX"));
    FlechaX->SetupAttachment(this);
	FlechaX->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));

	FlechaY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaY"));
    FlechaY->SetupAttachment(this);
	FlechaY->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));
	FlechaY->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	FlechaZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaZ"));
    FlechaZ->SetupAttachment(this);
	FlechaZ->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));
	FlechaZ->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlechaAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Flecha.Flecha'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (FlechaAsset.Succeeded()) {
        FlechaX->SetStaticMesh(FlechaAsset.Object);
        FlechaY->SetStaticMesh(FlechaAsset.Object);
        FlechaZ->SetStaticMesh(FlechaAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
            FlechaX->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
            FlechaY->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
            FlechaZ->SetMaterial(0, FlechaZMaterialAsset.Object);
        }
    }
	Origen->bRenderCustomDepth = true;
	FlechaX->bRenderCustomDepth = true;
	FlechaY->bRenderCustomDepth = true;
	FlechaZ->bRenderCustomDepth = true;
	Origen->SetCustomDepthStencilValue(255);
	FlechaX->SetCustomDepthStencilValue(254);
	FlechaY->SetCustomDepthStencilValue(252);
	FlechaZ->SetCustomDepthStencilValue(253);
	
}


// Called when the game starts
void UTransformacionWidget::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTransformacionWidget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTransformacionWidget::MostrarWidget() {
	Origen->SetVisibility(true);
	FlechaX->SetVisibility(true);
	FlechaY->SetVisibility(true);
	FlechaZ->SetVisibility(true);
}

void UTransformacionWidget::OcultarWidget() {
	Origen->SetVisibility(false);
	FlechaX->SetVisibility(false);
	FlechaY->SetVisibility(false);
	FlechaZ->SetVisibility(false);
}

void UTransformacionWidget::MostrarResaltado() {
	Origen->SetCustomDepthStencilValue(255);
	FlechaX->SetCustomDepthStencilValue(254);
	FlechaY->SetCustomDepthStencilValue(252);
	FlechaZ->SetCustomDepthStencilValue(253);
}

void UTransformacionWidget::OculatarResaltado() {
	Origen->SetCustomDepthStencilValue(0);
	FlechaX->SetCustomDepthStencilValue(0);
	FlechaY->SetCustomDepthStencilValue(0);
	FlechaZ->SetCustomDepthStencilValue(0);
}
//podria agregar por flechita o elemento, en caso de que los use como efecto de interaccion

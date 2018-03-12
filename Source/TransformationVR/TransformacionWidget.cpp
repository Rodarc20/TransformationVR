// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformacionWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"

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
	//FlechaY->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	FlechaZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaZ"));
    FlechaZ->SetupAttachment(this);
	FlechaZ->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));
	FlechaZ->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	//FlechaZ->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlechaAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Flecha.Flecha'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (FlechaAsset.Succeeded()) {
        FlechaX->SetStaticMesh(FlechaAsset.Object);
        FlechaY->SetStaticMesh(FlechaAsset.Object);
        FlechaZ->SetStaticMesh(FlechaAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
			EjeXNormal = FlechaXMaterialAsset.Object;
            FlechaX->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
			EjeYNormal = FlechaYMaterialAsset.Object;
            FlechaY->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
			EjeZNormal = FlechaZMaterialAsset.Object;
            FlechaZ->SetMaterial(0, FlechaZMaterialAsset.Object);
        }
    }

	ArcoX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcoX"));
    ArcoX->SetupAttachment(this);
	ArcoX->SetRelativeScale3D(FVector(0.58f, 0.58f, 0.58f));
	ArcoX->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	ArcoY= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcoY"));
    ArcoY->SetupAttachment(this);
	ArcoY->SetRelativeScale3D(FVector(0.58f, 0.58f, 0.58f));
	ArcoY->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	ArcoZ= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcoZ"));
    ArcoZ->SetupAttachment(this);
	ArcoZ->SetRelativeScale3D(FVector(0.58f, 0.58f, 0.58f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArcoAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Arco.Arco'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ArcoAsset.Succeeded()) {
        ArcoX->SetStaticMesh(ArcoAsset.Object);
        ArcoY->SetStaticMesh(ArcoAsset.Object);
        ArcoZ->SetStaticMesh(ArcoAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
            ArcoX->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
            ArcoY->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
            ArcoZ->SetMaterial(0, FlechaZMaterialAsset.Object);
        }
    }

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaXResaltado.FlechaXResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaXMaterialAsset.Succeeded()) {
		EjeXSelected = FlechaXMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZResaltado.FlechaZResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaYMaterialAsset.Succeeded()) {
		EjeYSelected = FlechaYMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaYResaltado.FlechaYResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaZMaterialAsset.Succeeded()) {
		EjeZSelected = FlechaZMaterialAsset.Object;
	}


	
    FlechaX->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaY->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaZ->SetCollisionProfileName(FName(TEXT("Transformacion")));
    ArcoX->SetCollisionProfileName(FName(TEXT("Transformacion")));
    ArcoY->SetCollisionProfileName(FName(TEXT("Transformacion")));
    ArcoZ->SetCollisionProfileName(FName(TEXT("Transformacion")));



	Origen->bRenderCustomDepth = true;
	FlechaX->bRenderCustomDepth = true;
	FlechaY->bRenderCustomDepth = true;
	FlechaZ->bRenderCustomDepth = true;
	ArcoX->bRenderCustomDepth = true;
	ArcoY->bRenderCustomDepth = true;
	ArcoZ->bRenderCustomDepth = true;
	Origen->SetCustomDepthStencilValue(255);
	FlechaX->SetCustomDepthStencilValue(254);
	FlechaY->SetCustomDepthStencilValue(252);
	FlechaZ->SetCustomDepthStencilValue(253);
	ArcoX->SetCustomDepthStencilValue(254);
	ArcoY->SetCustomDepthStencilValue(252);
	ArcoZ->SetCustomDepthStencilValue(253);

	//OcultarWidgetOrigen();
	//OcultarWidgetTraslacion();
	OcultarWidgetRotacion();
	
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

void UTransformacionWidget::MostrarWidgetOrigen() {
	Origen->SetVisibility(true);
}

void UTransformacionWidget::OcultarWidgetOrigen() {
	Origen->SetVisibility(false);
}

void UTransformacionWidget::MostrarWidgetTraslacion() {
	FlechaX->SetVisibility(true);
	FlechaY->SetVisibility(true);
	FlechaZ->SetVisibility(true);
}

void UTransformacionWidget::OcultarWidgetTraslacion() {
	FlechaX->SetVisibility(false);
	FlechaY->SetVisibility(false);
	FlechaZ->SetVisibility(false);
}

void UTransformacionWidget::MostrarWidgetRotacion() {
	ArcoX->SetVisibility(true);
	ArcoY->SetVisibility(true);
	ArcoZ->SetVisibility(true);
}

void UTransformacionWidget::OcultarWidgetRotacion() {
	ArcoX->SetVisibility(false);
	ArcoY->SetVisibility(false);
	ArcoZ->SetVisibility(false);
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
bool UTransformacionWidget::ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact) {
	//UE_LOG(LogClass, Log, TEXT("Buscando Eje"));
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceComponenteX")), true);
	FHitResult Hit;
	if (ArcoX->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		//UE_LOG(LogClass, Log, TEXT("Encontrado X"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeX;
		return true;
	}
    ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceComponenteY")), true);
	if (ArcoY->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		//UE_LOG(LogClass, Log, TEXT("Encontrado Y"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeY;
		return true;
	}
    ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceComponenteZ")), true);
	if (ArcoZ->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		//UE_LOG(LogClass, Log, TEXT("Encontrado Z"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeZ;
		return true;
	}
	Impact = FVector::ZeroVector;
	EjeColisionado = ETransformacionEje::ENone;
	return false;
}

void UTransformacionWidget::SeleccionarEjeRotacion(ETransformacionEje Eje) {
    switch (Eje) {
        case ETransformacionEje::EEjeX: {
			ArcoX->SetMaterial(0, EjeXSelected);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			ArcoY->SetMaterial(0, EjeYSelected);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			ArcoZ->SetMaterial(0, EjeZSelected);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

void UTransformacionWidget::DeseleccionarEjeRotacion(ETransformacionEje Eje) {
    switch (Eje) {
        case ETransformacionEje::EEjeX: {
			ArcoX->SetMaterial(0, EjeXNormal);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			ArcoY->SetMaterial(0, EjeYNormal);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			ArcoZ->SetMaterial(0, EjeZNormal);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

void UTransformacionWidget::HabilitarEje(ETransformacionEje Eje) {
    switch (Eje) {
        case ETransformacionEje::EEjeX: {
            FlechaX->SetVisibility(true);//de pronto uso los pmateriales resaltados
			//FlechaX->SetMaterial(0, EjeYNormal);
            //o con materiales
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
            FlechaY->SetVisibility(true);
			//ArcoY->SetMaterial(0, EjeYNormal);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
            FlechaZ->SetVisibility(true);
			//ArcoZ->SetMaterial(0, EjeZNormal);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

void UTransformacionWidget::DeshabilitarEje(ETransformacionEje Eje) {
    switch (Eje) {
        case ETransformacionEje::EEjeX: {
            FlechaX->SetVisibility(false);//de pronto uso los pmateriales resaltados
			//FlechaX->SetMaterial(0, EjeYNormal);
            //o con materiales
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
            FlechaY->SetVisibility(false);
			//ArcoY->SetMaterial(0, EjeYNormal);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
            FlechaZ->SetVisibility(false);
			//ArcoZ->SetMaterial(0, EjeZNormal);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

void UTransformacionWidget::CopiarTransform() {
	TransformTemporal = GetComponentTransform();
}
//podria agregar por flechita o elemento, en caso de que los use como efecto de interaccion
FVector UTransformacionWidget::InterseccionLineaPlanoXZ(FVector PuntoRecta, FVector VectorRecta) {//retorna en espacio local, esto pero quiza sea conveniete que retorne en espacio glloba, convertir estarepuesta despues
	//recibe parametro en espacio global
	//y hace los calculos con el trasnfor copiado para convertirlo s en local
    //solo si esta frente al controlo, no por detras, corregir esto
	//PuntoRecta = GetComponentTransform().InverseTransformPosition(PuntoRecta);//trasnforma de global a local
	//VectorRecta = GetComponentTransform().InverseTransformVector(VectorRecta);//trasnforma de global a local
	
	PuntoRecta = TransformTemporal.InverseTransformPosition(PuntoRecta);//trasnforma de global a local
	VectorRecta = TransformTemporal.InverseTransformVector(VectorRecta);//trasnforma de global a local
    if (VectorRecta.Y == 0) {
        return FVector(-1.0f);
    }
    float t = -PuntoRecta.Y / VectorRecta.Y;
    if (t >= 0) {//estoy asumiend que el plano es x=0, pero como es en espacio gloabl cuando roto da problemas
        FVector PuntoInterseccion (PuntoRecta.X + t*VectorRecta.X, 0.0f, PuntoRecta.Z + t*VectorRecta.Z);
		return PuntoInterseccion;//devuelve en local para identificar el -1., que indica no encontrado, y para usarlo afuera
		//return GetComponentTransform().TransformPosition(PuntoInterseccion);//convierte de local a global//tal ve no deba convertirlo aqui, si noafuera, ya que nocesito el local para os calculos

    }
    return FVector(-1.0f);
}

FVector UTransformacionWidget::InterseccionLineaPlanoXY(FVector PuntoRecta, FVector VectorRecta) {//retorna en espacio local, esto pero quiza sea conveniete que retorne en espacio glloba, convertir estarepuesta despues
	PuntoRecta = TransformTemporal.InverseTransformPosition(PuntoRecta);//trasnforma de global a local
	VectorRecta = TransformTemporal.InverseTransformVector(VectorRecta);//trasnforma de global a local
    if (VectorRecta.Z == 0) {
        return FVector(-1.0f);
    }
    float t = -PuntoRecta.Z / VectorRecta.Z;
    if (t >= 0) {//estoy asumiend que el plano es x=0, pero como es en espacio gloabl cuando roto da problemas
        //FVector PuntoInterseccion (PuntoRecta.X + t*VectorRecta.X, PuntoRecta.Z + t*VectorRecta.Z, 0.0f);
        FVector PuntoInterseccion (PuntoRecta.X + t*VectorRecta.X, PuntoRecta.Y + t*VectorRecta.Y, 0.0f);
		return PuntoInterseccion;//devuelve en local para identificar el -1., que indica no encontrado, y para usarlo afuera
    }
    return FVector(-1.0f);
}

FVector UTransformacionWidget::InterseccionLineaPlanoYZ(FVector PuntoRecta, FVector VectorRecta) {//retorna en espacio local, esto pero quiza sea conveniete que retorne en espacio glloba, convertir estarepuesta despues
	PuntoRecta = TransformTemporal.InverseTransformPosition(PuntoRecta);//trasnforma de global a local
	VectorRecta = TransformTemporal.InverseTransformVector(VectorRecta);//trasnforma de global a local
    if (VectorRecta.X == 0) {
        return FVector(-1.0f);
    }
    float t = -PuntoRecta.X / VectorRecta.X;
    if (t >= 0) {//estoy asumiend que el plano es x=0, pero como es en espacio gloabl cuando roto da problemas
        FVector PuntoInterseccion (0.0f, PuntoRecta.Y + t*VectorRecta.Y, PuntoRecta.Z + t*VectorRecta.Z);
		return PuntoInterseccion;//devuelve en local para identificar el -1., que indica no encontrado, y para usarlo afuera
    }
    return FVector(-1.0f);
}

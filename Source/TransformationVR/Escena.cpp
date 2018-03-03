// Fill out your copyright notice in the Description page of Project Settings.

#include "Escena.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TransformacionWidget.h"
#include "Engine/StaticMesh.h"
#include "Casa.h"
#include "Components/WidgetComponent.h"


// Sets default values
AEscena::AEscena() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    //TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("Ejes"));

	Origen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Origen"));
    Origen->SetupAttachment(RootComponent);
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
    FlechaX->SetupAttachment(RootComponent);
	FlechaX->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));

	FlechaY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaY"));
    FlechaY->SetupAttachment(RootComponent);
	FlechaY->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaY->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	FlechaZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaZ"));
    FlechaZ->SetupAttachment(RootComponent);
	FlechaZ->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaZ->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	FlechaXNegative = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaXNegative"));
    FlechaXNegative->SetupAttachment(RootComponent);
	FlechaXNegative->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaXNegative->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	FlechaYNegative = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaYNegative"));
    FlechaYNegative->SetupAttachment(RootComponent);
	FlechaYNegative->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaYNegative->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	FlechaZNegative = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaZNegative"));
    FlechaZNegative->SetupAttachment(RootComponent);
	FlechaZNegative->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaZNegative->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlechaAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/FlechaGrande.FlechaGrande'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (FlechaAsset.Succeeded()) {
        FlechaX->SetStaticMesh(FlechaAsset.Object);
        FlechaY->SetStaticMesh(FlechaAsset.Object);
        FlechaZ->SetStaticMesh(FlechaAsset.Object);
        FlechaXNegative->SetStaticMesh(FlechaAsset.Object);
        FlechaYNegative->SetStaticMesh(FlechaAsset.Object);
        FlechaZNegative->SetStaticMesh(FlechaAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
			EjeXNormal = FlechaXMaterialAsset.Object;
            FlechaX->SetMaterial(0, FlechaXMaterialAsset.Object);
            FlechaXNegative->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
			EjeYNormal = FlechaYMaterialAsset.Object;
            FlechaY->SetMaterial(0, FlechaYMaterialAsset.Object);
            FlechaYNegative->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
			EjeZNormal = FlechaZMaterialAsset.Object;
            FlechaZ->SetMaterial(0, FlechaZMaterialAsset.Object);
            FlechaZNegative->SetMaterial(0, FlechaZMaterialAsset.Object);
        }
    }

    FlechaX->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaY->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaZ->SetCollisionProfileName(FName(TEXT("Transformacion")));

    FlechaX->OnComponentBeginOverlap.AddDynamic(this, &AEscena::OnBeginOverlapEjeX);
    FlechaX->OnComponentEndOverlap.AddDynamic(this, &AEscena::OnEndOverlapEjeX);
    FlechaY->OnComponentBeginOverlap.AddDynamic(this, &AEscena::OnBeginOverlapEjeY);
    FlechaY->OnComponentEndOverlap.AddDynamic(this, &AEscena::OnEndOverlapEjeY);
    FlechaZ->OnComponentBeginOverlap.AddDynamic(this, &AEscena::OnBeginOverlapEjeZ);
    FlechaZ->OnComponentEndOverlap.AddDynamic(this, &AEscena::OnEndOverlapEjeZ);

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXRMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaXResaltado.FlechaXResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaXRMaterialAsset.Succeeded()) {
		EjeXSelected = FlechaXRMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYRMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaYResaltado.FlechaYResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaYRMaterialAsset.Succeeded()) {
		EjeYSelected = FlechaYRMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZRMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZResaltado.FlechaZResaltado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaZRMaterialAsset.Succeeded()) {
		EjeZSelected = FlechaZRMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXTMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaXTitilando.FlechaXTitilando'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaXTMaterialAsset.Succeeded()) {
		EjeXTitilando = FlechaXTMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYTMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaYTitilando.FlechaYTitilando'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaYTMaterialAsset.Succeeded()) {
		EjeYTitilando = FlechaYTMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZTMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZTitilando.FlechaZTitilando'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaZTMaterialAsset.Succeeded()) {
		EjeZTitilando = FlechaZTMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXPMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaXPresionado.FlechaXPresionado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaXPMaterialAsset.Succeeded()) {
		EjeXPresionado = FlechaXPMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYPMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaYPresionado.FlechaYPresionado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaYPMaterialAsset.Succeeded()) {
		EjeYPresionado = FlechaYPMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZPMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZPresionado.FlechaZPresionado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaZPMaterialAsset.Succeeded()) {
		EjeZPresionado = FlechaZPMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXIMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaXInhabilitado.FlechaXInhabilitado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaXIMaterialAsset.Succeeded()) {
		EjeXInhabilitado = FlechaXIMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYIMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaYInhabilitado.FlechaYInhabilitado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaYIMaterialAsset.Succeeded()) {
		EjeYInhabilitado = FlechaYIMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZIMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZInhabilitado.FlechaZInhabilitado'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
	if (FlechaZIMaterialAsset.Succeeded()) {
		EjeZInhabilitado = FlechaZIMaterialAsset.Object;
	}

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraXClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraX.LetraX_C'"));
    LetraX = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraX"));
    LetraX->SetupAttachment(RootComponent);
    LetraX->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraX->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraX->SetPivot(FVector2D(0.5f, 0.5f));
    LetraX->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));
    LetraX->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraX->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraXClass.Succeeded()) {
        LetraX->SetWidgetClass(LetraXClass.Class);
    }
    LetraX->SetVisibility(false);

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraYClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraY.LetraY_C'"));
    LetraY = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraY"));
    LetraY->SetupAttachment(RootComponent);
    LetraY->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraY->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraY->SetPivot(FVector2D(0.5f, 0.5f));
    LetraY->SetRelativeLocation(FVector(0.0f, 70.0f, 0.0f));
    LetraY->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraY->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraYClass.Succeeded()) {
        LetraY->SetWidgetClass(LetraYClass.Class);
    }
    LetraY->SetVisibility(false);

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraZClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraZ.LetraZ_C'"));
    LetraZ = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraZ"));
    LetraZ->SetupAttachment(RootComponent);
    LetraZ->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraZ->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraZ->SetPivot(FVector2D(0.5f, 0.5f));
    LetraZ->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    LetraZ->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraZ->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraZClass.Succeeded()) {
        LetraZ->SetWidgetClass(LetraZClass.Class);
    }
    LetraZ->SetVisibility(false);

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraXNClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraXN.LetraXN_C'"));
    LetraXN = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraXN"));
    LetraXN->SetupAttachment(RootComponent);
    LetraXN->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraXN->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraXN->SetPivot(FVector2D(0.5f, 0.5f));
    LetraXN->SetRelativeLocation(FVector(-70.0f, 0.0f, 0.0f));
    LetraXN->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraXN->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraXNClass.Succeeded()) {
        LetraXN->SetWidgetClass(LetraXNClass.Class);
    }
    LetraXN->SetVisibility(false);

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraYNClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraYN.LetraYN_C'"));
    LetraYN = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraYN"));
    LetraYN->SetupAttachment(RootComponent);
    LetraYN->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraYN->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraYN->SetPivot(FVector2D(0.5f, 0.5f));
    LetraYN->SetRelativeLocation(FVector(0.0f, -70.0f, 0.0f));
    LetraYN->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraYN->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraYNClass.Succeeded()) {
        LetraYN->SetWidgetClass(LetraYNClass.Class);
    }
    LetraYN->SetVisibility(false);

    static ConstructorHelpers::FClassFinder<UUserWidget> LetraZNClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LetraZN.LetraZN_C'"));
    LetraZN = CreateDefaultSubobject<UWidgetComponent>(TEXT("LetraZN"));
    LetraZN->SetupAttachment(RootComponent);
    LetraZN->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    LetraZN->SetDrawSize(FVector2D(200.0f, 200.0f));
    LetraZN->SetPivot(FVector2D(0.5f, 0.5f));
    LetraZN->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    LetraZN->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    LetraZN->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (LetraZNClass.Succeeded()) {
        LetraZN->SetWidgetClass(LetraZNClass.Class);
    }
    LetraZN->SetVisibility(false);

    OcultarWidget();
}

// Called when the game starts or when spawned
void AEscena::BeginPlay() {
	Super::BeginPlay();
	
    EnableInput(GetWorld()->GetFirstPlayerController());
    bBlockInput = 0;
    //AutoReceiveInput = EAutoReceiveInput::Player0;
    
     //UInputComponent* myInputComp = this->InputComponent;  //InputComponent variable is from AActor.h
     //check that it is valid before calling
     //if (myInputComp) {
    if(this->InputComponent){
         //SetupPlayerInputComponent(myInputComp);
         SetupPlayerInputComponent(this->InputComponent);
     }

    TArray<AActor *> CasasEncontradas;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACasa::StaticClass(), CasasEncontradas);
    //UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
    if (CasasEncontradas.Num()) {
        ACasa * const CasaEncontrada = Cast<ACasa>(CasasEncontradas[0]);
        if (CasaEncontrada) {
            UE_LOG(LogClass, Log, TEXT("Casa para escena encontrada"));
            Casa = CasaEncontrada;
        }
    }
}

// Called every frame
void AEscena::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


    //verificar si alguno de los ejes esta siendo tocado por algun control
    //debo buscar coincidencia, es decir debo asegurarme de estar si algun eje se toca, si es con el control derecho debe ser seleccionado solo cuando se presione grabRight
}

void AEscena::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {
    //PlayerInputComponent->bBlockInput = 0;
    PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AEscena::GrabRightPressed).bConsumeInput = 0;
    PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &AEscena::GrabRightReleased).bConsumeInput = 0;

    PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AEscena::GrabLeftPressed).bConsumeInput = 0;
    PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &AEscena::GrabLeftReleased).bConsumeInput = 0;
}
//debo tenerlos efectos de eje presseda, eje birando, eje seleccionado, y cuando se selecciona al soltar el grab, debo mndar la señal de seleccionar eje en la casa

void AEscena::NormalizarEje(ETransformacionEje EjeNormalizado) {
    switch (EjeNormalizado) {
        case ETransformacionEje::EEjeX: {
			FlechaX->SetMaterial(0, EjeXNormal);
        }
        break;
        case ETransformacionEje::EEjeY: {
			FlechaY->SetMaterial(0, EjeYNormal);
        }
        break;
        case ETransformacionEje::EEjeZ: {
			FlechaZ->SetMaterial(0, EjeZNormal);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::SeleccionarEje(ETransformacionEje EjeSeleccionado) {
    switch (EjeSeleccionado) {
        case ETransformacionEje::EEjeX: {
			FlechaX->SetMaterial(0, EjeXSelected);
        }
        break;
        case ETransformacionEje::EEjeY: {
			FlechaY->SetMaterial(0, EjeYSelected);
        }
        break;
        case ETransformacionEje::EEjeZ: {
			FlechaZ->SetMaterial(0, EjeZSelected);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::PresionarEje(ETransformacionEje EjePresionado) {
    switch (EjePresionado) {
        case ETransformacionEje::EEjeX: {
			FlechaX->SetMaterial(0, EjeXPresionado);
        }
        break;
        case ETransformacionEje::EEjeY: {
			FlechaY->SetMaterial(0, EjeYPresionado);
        }
        break;
        case ETransformacionEje::EEjeZ: {
			FlechaZ->SetMaterial(0, EjeZPresionado);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::TitilarEje(ETransformacionEje EjeTitilado) {
    switch (EjeTitilado) {
        case ETransformacionEje::EEjeX: {
			FlechaX->SetMaterial(0, EjeXTitilando);
        }
        break;
        case ETransformacionEje::EEjeY: {
			FlechaY->SetMaterial(0, EjeYTitilando);
        }
        break;
        case ETransformacionEje::EEjeZ: {
			FlechaZ->SetMaterial(0, EjeZTitilando);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::InhabilitarEje(ETransformacionEje EjeInhabilitado) {
    switch (EjeInhabilitado) {
        case ETransformacionEje::EEjeX: {
			FlechaX->SetMaterial(0, EjeXInhabilitado);
        }
        break;
        case ETransformacionEje::EEjeY: {
			FlechaY->SetMaterial(0, EjeYInhabilitado);
        }
        break;
        case ETransformacionEje::EEjeZ: {
			FlechaZ->SetMaterial(0, EjeZInhabilitado);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::SeleccionEje(ETransformacionEje EjeSeleccionar) {
    switch (EjeSeleccionar) {
        case ETransformacionEje::EEjeX: {
            SeleccionarEje(ETransformacionEje::EEjeX);
            MostrarEjeNegativo(ETransformacionEje::EEjeX);
            InhabilitarEje(ETransformacionEje::EEjeY);
            InhabilitarEje(ETransformacionEje::EEjeZ);
            OcultarEjeNegativo(ETransformacionEje::EEjeY);
            OcultarEjeNegativo(ETransformacionEje::EEjeZ);
        }
        break;
        case ETransformacionEje::EEjeY: {
            SeleccionarEje(ETransformacionEje::EEjeY);
            MostrarEjeNegativo(ETransformacionEje::EEjeY);
            InhabilitarEje(ETransformacionEje::EEjeX);
            InhabilitarEje(ETransformacionEje::EEjeZ);
            OcultarEjeNegativo(ETransformacionEje::EEjeX);
            OcultarEjeNegativo(ETransformacionEje::EEjeZ);
        }
        break;
        case ETransformacionEje::EEjeZ: {
            SeleccionarEje(ETransformacionEje::EEjeZ);
            MostrarEjeNegativo(ETransformacionEje::EEjeZ);
            InhabilitarEje(ETransformacionEje::EEjeX);
            InhabilitarEje(ETransformacionEje::EEjeY);
            OcultarEjeNegativo(ETransformacionEje::EEjeX);
            OcultarEjeNegativo(ETransformacionEje::EEjeY);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::MostrarEjeNegativo(ETransformacionEje EjeNegativo) {
    switch (EjeNegativo) {
        case ETransformacionEje::EEjeX: {
            FlechaXNegative->SetVisibility(true);
            LetraXN->SetVisibility(true);
        }
        break;
        case ETransformacionEje::EEjeY: {
            FlechaYNegative->SetVisibility(true);
            LetraYN->SetVisibility(true);
        }
        break;
        case ETransformacionEje::EEjeZ: {
            FlechaZNegative->SetVisibility(true);
            LetraZN->SetVisibility(true);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::OcultarEjeNegativo(ETransformacionEje EjeNegativo) {
    switch (EjeNegativo) {
        case ETransformacionEje::EEjeX: {
            FlechaXNegative->SetVisibility(false);
            LetraXN->SetVisibility(false);
        }
        break;
        case ETransformacionEje::EEjeY: {
            FlechaYNegative->SetVisibility(false);
            LetraYN->SetVisibility(false);
        }
        break;
        case ETransformacionEje::EEjeZ: {
            FlechaZNegative->SetVisibility(false);
            LetraZN->SetVisibility(false);
        }
        break;
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;
    }
}

void AEscena::GrabRightPressed() {
    if (OverlapedRightEjes.Num()) {
        PresionarEje(OverlapedRightEjes[0]);
    }
}

void AEscena::GrabRightTick() {
}

void AEscena::GrabRightReleased() {
    if (OverlapedRightEjes.Num() && Casa) {
        Casa->SetTransformacionEje(OverlapedRightEjes[0]);

        SeleccionEje(OverlapedRightEjes[0]);
        //deberia refresar a estado normal o inhabilitado los otros ejes
    }
}

void AEscena::GrabLeftPressed() {
    if (OverlapedLeftEjes.Num()) {
        PresionarEje(OverlapedLeftEjes[0]);
    }
}

void AEscena::GrabLeftTick() {
}

void AEscena::GrabLeftReleased() {
    if (OverlapedLeftEjes.Num() && Casa) {
        Casa->SetTransformacionEje(OverlapedLeftEjes[0]);
        SeleccionarEje(OverlapedLeftEjes[0]);
    }
}

void AEscena::MostrarWidget() {
	Origen->SetVisibility(true);
	FlechaX->SetVisibility(true);
	FlechaY->SetVisibility(true);
	FlechaZ->SetVisibility(true);
	FlechaXNegative->SetVisibility(false);
	FlechaYNegative->SetVisibility(false);
	FlechaZNegative->SetVisibility(false);
	LetraX->SetVisibility(true);
	LetraY->SetVisibility(true);
	LetraZ->SetVisibility(true);
	LetraXN->SetVisibility(false);
	LetraYN->SetVisibility(false);
	LetraZN->SetVisibility(false);
    TitilarEje(ETransformacionEje::EEjeX);
    TitilarEje(ETransformacionEje::EEjeY);
    TitilarEje(ETransformacionEje::EEjeZ);
}

void AEscena::OcultarWidget() {
	Origen->SetVisibility(false);
	FlechaX->SetVisibility(false);
	FlechaY->SetVisibility(false);
	FlechaZ->SetVisibility(false);
	FlechaXNegative->SetVisibility(false);
	FlechaYNegative->SetVisibility(false);
	FlechaZNegative->SetVisibility(false);
	LetraX->SetVisibility(false);
	LetraY->SetVisibility(false);
	LetraZ->SetVisibility(false);
	LetraXN->SetVisibility(false);
	LetraYN->SetVisibility(false);
	LetraZN->SetVisibility(false);
    NormalizarEje(ETransformacionEje::EEjeX);
    NormalizarEje(ETransformacionEje::EEjeY);
    NormalizarEje(ETransformacionEje::EEjeZ);
}

void AEscena::OnBeginOverlapEjeX(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.AddUnique(ETransformacionEje::EEjeX);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje X con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.AddUnique(ETransformacionEje::EEjeX);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje Y con ControllerLeft"));
                    }
                }
            }
        }
    }
}

void AEscena::OnBeginOverlapEjeY(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.AddUnique(ETransformacionEje::EEjeY);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje Y con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.AddUnique(ETransformacionEje::EEjeY);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje Y con ControllerLeft"));
                    }
                }
            }
        }
    }
}

void AEscena::OnBeginOverlapEjeZ(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.AddUnique(ETransformacionEje::EEjeZ);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje Z con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.AddUnique(ETransformacionEje::EEjeZ);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlaped Eje Z con ControllerLeft"));
                    }
                }
            }
        }
    }
}

void AEscena::OnEndOverlapEjeX(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.Remove(ETransformacionEje::EEjeX);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje X con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.Remove(ETransformacionEje::EEjeX);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje X con ControllerLeft"));
                    }
                }
            }
        }
    }
}

void AEscena::OnEndOverlapEjeY(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.Remove(ETransformacionEje::EEjeY);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje Y con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.Remove(ETransformacionEje::EEjeY);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje Y con ControllerLeft"));
                    }
                }
            }
        }
    }
}

void AEscena::OnEndOverlapEjeZ(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (Casa && Casa->GetCasaTask() == EVRCasaTask::EPlayTask) {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
            AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
            if (VRPawn && !VRPawn->IsPendingKill()) {
                UCapsuleComponent * const ColisionController = Cast<UCapsuleComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                if (ColisionController) {
                    if (ColisionController->GetName() == "ColisionControllerRight") {
                        OverlapedRightEjes.Remove(ETransformacionEje::EEjeZ);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje Z con ControllerRight"));
                    }
                    else if (ColisionController->GetName() == "ColisionControllerLeft") {
                        OverlapedLeftEjes.Remove(ETransformacionEje::EEjeZ);
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlaped Eje Z con ControllerLeft"));
                    }
                }
            }
        }
    }
}

//voy a tener dos array, uno para el control derecho y otro para el control izquierdo
//con el cual estas funciones lo unico que hace ens ingresar su respectivo eje a esos arrays cuando entren en contacto con el control correspondiente a cada array
//y lo saco el eje, cuando el control deje de estar en contacto
//entonces cuando presione el trigger de cada no, debo detectar si en el array ahay algo, pouedo ahcer la jugada de coger el que esta mas cerca, pero por ahora seleccionare solo el primer elemento
//estos array no contendran el eje, lo que contendra sera el enum, para asi manejar todo solo pasando ese parametro, igual que los ejectos


//necestio estados para los efectos visuales de los ejes, los cuales funcionan, en distintas partes de esta clase, solo la animacion del titilando estara controlada en el tick
//aun que tambien los materiales puede ejecutar esa animacion, depende de como lo quieras manejar, lo mejor seria que sea independiente
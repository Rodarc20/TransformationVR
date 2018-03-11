// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Parte.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Robot.h"
#include "Jerarquia.h"
#include "Bloque.h"
#include "Casa.h"
#include <limits>




// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
    VRCamera->SetupAttachment(RootComponent);

    ColisionHead = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionHead"));
    ColisionHead->SetupAttachment(VRCamera);
    ColisionHead->SetRelativeLocation(FVector::ZeroVector);
    ColisionHead->InitSphereRadius(20.0f);

    MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
    MotionControllerLeft->SetupAttachment(RootComponent);
    MotionControllerLeft->SetRelativeLocation(FVector(50.0f, -40.0f, -20.0f));
    MotionControllerLeft->Hand = EControllerHand::Left;

    MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
    MotionControllerRight->SetupAttachment(RootComponent);
    MotionControllerRight->SetRelativeLocation(FVector(50.0f, 40.0f, 0.0f));
    MotionControllerRight->Hand = EControllerHand::Right;

    ViveControllerLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ViveControllerLeft"));
    ViveControllerLeft->SetupAttachment(MotionControllerLeft);
    ViveControllerLeft->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));//ver cual es el yaw y el roll

    ViveControllerRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ViveControllerRight"));
    ViveControllerRight->SetupAttachment(MotionControllerRight);
    ViveControllerRight->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));


    static ConstructorHelpers::FObjectFinder<UStaticMesh> ViveControllerMeshAsset(TEXT("StaticMesh'/Game/ControllerSetup/vr_controller_vive_1_5.vr_controller_vive_1_5'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (ViveControllerMeshAsset.Succeeded()) {
        ViveControllerLeft->SetStaticMesh(ViveControllerMeshAsset.Object);
        ViveControllerRight->SetStaticMesh(ViveControllerMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> ViveControllerMaterialAsset(TEXT("Material'/Game/ControllerSetup/ControlMaterial.ControlMaterial'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (ViveControllerMaterialAsset.Succeeded()) {
            ViveControllerLeft->SetMaterial(0, ViveControllerMaterialAsset.Object);
            ViveControllerRight->SetMaterial(0, ViveControllerMaterialAsset.Object);
        }
    }

	ColisionControllerLeft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColisionControllerLeft"));
	ColisionControllerLeft->SetupAttachment(MotionControllerLeft);
	ColisionControllerLeft->InitCapsuleSize(5.0f, 5.0f);
	ColisionControllerLeft->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
	ColisionControllerLeft->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    ColisionControllerLeft->OnComponentBeginOverlap.AddDynamic(this, &AVRPawn::OnBeginOverlapControllerLeft);
    ColisionControllerLeft->OnComponentEndOverlap.AddDynamic(this, &AVRPawn::OnEndOverlapControllerLeft);

	ColisionControllerRight = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColisionControllerRight"));
	ColisionControllerRight->SetupAttachment(MotionControllerRight);
	ColisionControllerRight->InitCapsuleSize(5.0f, 5.0f);
	ColisionControllerRight->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
	ColisionControllerRight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    ColisionControllerRight->OnComponentBeginOverlap.AddDynamic(this, &AVRPawn::OnBeginOverlapControllerRight);
    ColisionControllerRight->OnComponentEndOverlap.AddDynamic(this, &AVRPawn::OnEndOverlapControllerRight);


    Interaction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Interaction"));
    Interaction->SetupAttachment(MotionControllerRight);
    //Interaction->bShowDebug = true;

    Movimiento = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movimiento"));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> LaserAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserAzul.LaserAzul'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    Laser = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Laser"));
    Laser->SetupAttachment(MotionControllerRight);
    if (LaserAsset.Succeeded()) {
        Laser->SetTemplate(LaserAsset.Object);
        Lasers.Add(LaserAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserVerde.LaserVerde'"));
    if (LasersAsset.Succeeded()) {
        Lasers.Add(LasersAsset.Object);
    }
    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset2(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserRojo.LaserRojo'"));
    if (LasersAsset2.Succeeded()) {
        Lasers.Add(LasersAsset2.Object);
    }
    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset3(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserAmarillo.LaserAmarillo'"));
    if (LasersAsset3.Succeeded()) {
        Lasers.Add(LasersAsset3.Object);
    }
    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset4(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserTurquesa.LaserTurquesa'"));
    if (LasersAsset4.Succeeded()) {
        Lasers.Add(LasersAsset4.Object);
    }
    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset5(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserNaranja.LaserNaranja'"));
    if (LasersAsset5.Succeeded()) {
        Lasers.Add(LasersAsset5.Object);
    }
    static ConstructorHelpers::FObjectFinder<UParticleSystem> LasersAsset6(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserBeam/LaserMorado.LaserMorado'"));
    if (LasersAsset6.Succeeded()) {
        Lasers.Add(LasersAsset6.Object);
    }

    EfectoImpacto = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoImpacto"));
    EfectoImpacto->SetupAttachment(MotionControllerRight);
    //static ConstructorHelpers::FObjectFinder<UParticleSystem> EfectoImpactoAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserImpact/LaserImpactRotacion.LaserImpactRotacion'"));
    //static ConstructorHelpers::FObjectFinder<UParticleSystem> EfectoImpactoAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserImpact/LaserEImpactRotacion.LaserEImpactRotacion'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> EfectoImpactoAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserImpact/LaserEImpactRotacion1.LaserEImpactRotacion1'"));
    if (EfectoImpactoAsset.Succeeded()) {
        EfectoImpacto->SetTemplate(EfectoImpactoAsset.Object);
    }
    EfectoImpacto->SetRelativeLocation(FVector::ZeroVector);
    EfectoImpacto->Deactivate();

    PuntoReferenciaRight = CreateDefaultSubobject<USceneComponent>(TEXT("PuntoReferenciaRight"));
	PuntoReferenciaRight->SetupAttachment(MotionControllerRight);
	PuntoReferenciaRight->SetRelativeLocation(FVector::ZeroVector);

    PuntoReferenciaLeft = CreateDefaultSubobject<USceneComponent>(TEXT("PuntoReferenciaLeft"));
	PuntoReferenciaLeft->SetupAttachment(MotionControllerLeft);
	PuntoReferenciaLeft->SetRelativeLocation(FVector::ZeroVector);

    static ConstructorHelpers::FClassFinder<UUserWidget> PadValoresClass(TEXT("WidgetBlueprint'/Game/Trasnformation/UMG/PadValor.PadValor_C'"));
    PadValores = CreateDefaultSubobject<UWidgetComponent>(TEXT("PadValores"));
    PadValores->SetWidgetSpace(EWidgetSpace::World);
    PadValores->SetupAttachment(MotionControllerRight);
    PadValores->SetRelativeLocation(FVector(-5.0f, -6.0f, 0.0f));
    PadValores->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
    PadValores->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (PadValoresClass.Succeeded()) {
        PadValores->SetWidgetClass(PadValoresClass.Class);
    }
    PadValores->SetDrawSize(FVector2D(160.0f, 80.0f));
    PadValores->SetPivot(FVector2D(0.5f, 0.5f));
    //PadValores->SetVisibility(false);

    Velocidad = 200.0f;
    bPadDerecho = false;
    bPadIzquierdo = false;
    LaserIndice = 0;

	bGrabRightParte = false;
	bGrabLeftParte = false;
	bRootEstablecida = false;

	bBuscarParteLeft = true;
	bBuscarParteRight = true;

	bBuscarBloqueLeft = true;
	bBuscarBloqueRight = true;

	CurrentJerarquiaTask = EVRJerarquiaTask::EArmarTask;
	//CurrentJerarquiaTask = EVRJerarquiaTask::ENoTask;
	//CurrentCasaTask = EVRCasaTask::ENoTask;
	CurrentCasaTask = EVRCasaTask::ENoTask;
    //deberia actualizarse a medida que cambien las tareas en el robot
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float MX = GetInputAxisValue("MoveForward");
    float MY = GetInputAxisValue("MoveRight");
    float MZ = GetInputAxisValue("MoveUp");
    if (bPadIzquierdo && (MX != 0.0f || MY != 0.0f || MZ != 0.0f)) {
        //FVector Desplazamiento = GetActorForwardVector() * MX + GetActorRightVector() * MY + GetActorUpVector() * MZ;
        //FVector Desplazamiento = VRCamera->GetForwardVector() * MX + VRCamera->GetRightVector() * MY + VRCamera->GetUpVector() * MZ;
        FVector Adelante = VRCamera->GetForwardVector();
        Adelante.Z = 0;
        FVector Derecha = VRCamera->GetRightVector();
        Derecha.Z = 0;
        FVector Desplazamiento = Adelante.GetSafeNormal() * MX + Derecha.GetSafeNormal() * MY + GetActorUpVector() * MZ;
        //AddMovementInput(Des)
        SetActorLocation(GetActorLocation() + Desplazamiento.GetSafeNormal() * Velocidad * DeltaTime);
    }

    FRotator RotationCamera = VRCamera->GetRelativeTransform().GetRotation().Rotator();
    FRotator NewRotationCamera (RotationCamera.Pitch + GetInputAxisValue("CameraUp"), RotationCamera.Yaw + GetInputAxisValue("CameraRight"), RotationCamera.Roll);
    VRCamera->SetRelativeRotation(NewRotationCamera);

    float CX = GetInputAxisValue("ControllerForward");
    float CY = GetInputAxisValue("ControllerRight");
    float CZ = GetInputAxisValue("ControllerUp");
    if (CX != 0.0f || CY != 0.0f || CZ != 0.0f) {
        FVector Desplazamiento (CX, CY, CZ);
        //MotionControllerRight->SetWorldLocation(MotionControllerRight->GetComponentLocation() + Desplazamiento.GetSafeNormal() * Velocidad/2 * DeltaTime);
        MotionControllerRight->SetRelativeLocation(MotionControllerRight->GetRelativeTransform().GetLocation() + Desplazamiento.GetSafeNormal() * Velocidad/2 * DeltaTime);
    }
    float CRoll = GetInputAxisValue("ControllerRoll");
    float CPitch = GetInputAxisValue("ControllerPitch");
    float CYaw = GetInputAxisValue("ControllerYaw");
    if (CRoll != 0.0f || CPitch != 0.0f || CYaw != 0.0f) {
        FRotator Rotacion(CPitch, CYaw, CRoll);
        MotionControllerRight->AddRelativeRotation(Rotacion);
    }

	GrabRightTick();//esta funcion , funciona mal
	GrabLeftTick();

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAction("Moverse", IE_Pressed, this, &AVRPawn::PadIzquierdoPressed);
    InputComponent->BindAction("Moverse", IE_Released, this, &AVRPawn::PadIzquierdoReleased);

    InputComponent->BindAction("IngresarValor", IE_Pressed, this, &AVRPawn::PadDerechoPressed);
    InputComponent->BindAction("IngresarValor", IE_Released, this, &AVRPawn::PadDerechoReleased);
    //InputComponent->BindAction("Seleccionar", IE_Pressed, this, &AVRPawn::SelectPressed);
    //InputComponent->BindAction("Seleccionar", IE_Released, this, &AVRPawn::SelectReleased);
    InputComponent->BindAction("GrabRight", IE_Pressed, this, &AVRPawn::GrabRightPressed);
    InputComponent->BindAction("GrabRight", IE_Released, this, &AVRPawn::GrabRightReleased);
    InputComponent->BindAction("GrabLeft", IE_Pressed, this, &AVRPawn::GrabLeftPressed);
    InputComponent->BindAction("GrabLeft", IE_Released, this, &AVRPawn::GrabLeftReleased);

    InputComponent->BindAxis("MoveForward");
    InputComponent->BindAxis("MoveRight");
    InputComponent->BindAxis("MoveUp");

    InputComponent->BindAxis("ControllerForward");
    InputComponent->BindAxis("ControllerRight");
    InputComponent->BindAxis("ControllerUp");

    InputComponent->BindAxis("ControllerRoll");
    InputComponent->BindAxis("ControllerPitch");
    InputComponent->BindAxis("ControllerYaw");

    InputComponent->BindAxis("CameraUp");
    InputComponent->BindAxis("CameraRight");

    InputComponent->BindAxis("AumentarValor", this, &AVRPawn::AumentarValor);

    //es probable que no use el puntero laser, por lo tanto no existira select, con esa funcionalidad
    //pero si usare el triger para sostener objetos
}

void AVRPawn::CambiarLaser(int Indice) {
    if (Indice < Lasers.Num()) {
        LaserIndice = Indice;
        Laser->SetTemplate(Lasers[Indice]);
    }
}

void AVRPawn::MostrarLaser() {
    Laser->SetVisibility(true);
    Laser->ActivateSystem();
}

void AVRPawn::OcultarLaser() {
    Laser->SetVisibility(false);
    Laser->DeactivateSystem();
}

void AVRPawn::MostrarImpactoLaser() {
    EfectoImpacto->SetVisibility(true);
    EfectoImpacto->ActivateSystem();
}

void AVRPawn::OcultarImpactoLaser() {
    EfectoImpacto->SetVisibility(false);
    EfectoImpacto->DeactivateSystem();
}

void AVRPawn::CambiarPuntoFinal(FVector PuntoFinal) {
	//Laser->SetBeamTargetPoint(0, PuntoFinal, 0);//o target
    if (Interaction->IsOverHitTestVisibleWidget()) {
        FHitResult HitInteraction = Interaction->GetLastHitResult();
        if ((MotionControllerRight->GetComponentLocation() - HitInteraction.ImpactPoint).Size() < (MotionControllerRight->GetComponentLocation() - PuntoFinal).Size()) {
            Laser->SetBeamTargetPoint(0, HitInteraction.ImpactPoint, 0);//o target
        }
    }
    else {
        //Laser->SetBeamEndPoint(0, PuntoFinal);//o target
        Laser->SetBeamTargetPoint(0, PuntoFinal, 0);//o target
        //Laser->SetBeamTargetPoint(0, GetTransform().InverseTransformPosition(PuntoFinal), 0);//o target
    }
}

int AVRPawn::LaserActual() {
    return LaserIndice;
}

void AVRPawn::SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask) {
	CurrentJerarquiaTask = NewJerarquiaTask;
	//UE_LOG(LogClass, Log, TEXT("Cambiando Jerarquia Task VRPawn %d"), NewJerarquiaTask);

    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
            OcultarLaser();
            OcultarImpactoLaser();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
            MostrarLaser();
            MostrarImpactoLaser();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
            OcultarLaser();
            OcultarImpactoLaser();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
            OcultarLaser();
            OcultarImpactoLaser();
        }
        break;
    }

}

EVRJerarquiaTask AVRPawn::GetJerarquiaTask() {
	return CurrentJerarquiaTask;
}

void AVRPawn::SetCasaTask(EVRCasaTask NewCasaTask) {
    CurrentCasaTask = NewCasaTask;
	//UE_LOG(LogClass, Log, TEXT("Cambiando Casa Task VRPawn %d"), NewCasaTask);
    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            OcultarLaser();
            OcultarImpactoLaser();
        }
        break;
        case EVRCasaTask::EPlayTask: {
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
            OcultarLaser();
            OcultarImpactoLaser();
        }
        break;
    }
}

EVRCasaTask AVRPawn::GetCasaTask() {
    return CurrentCasaTask;
}

void AVRPawn::AumentarValor(float AxisValue) {
    if (bPadDerecho) {
        TArray<AActor *> CasasEncontradas;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACasa::StaticClass(), CasasEncontradas);
        //UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
        if (CasasEncontradas.Num()) {
            ACasa * const CasaEncontrada = Cast<ACasa>(CasasEncontradas[0]);
            if (CasaEncontrada) {
                UE_LOG(LogClass, Log, TEXT("Aumentando valor %f"), AxisValue);
                CasaEncontrada->ValorAplicar += AxisValue;//debiera estar delimitidao de alguna forma o con alguna velocidad
            }
        }
    }
}

void AVRPawn::PadDerechoPressed() {
    bPadDerecho = true;
}

void AVRPawn::PadDerechoReleased() {
    bPadDerecho = false;
}

void AVRPawn::PadIzquierdoPressed() {
    bPadIzquierdo = true;
}

void AVRPawn::PadIzquierdoReleased() {
    bPadIzquierdo = false;
}

void AVRPawn::SelectPressed() {
    //Interaction->PressPointerKey(EKeys::LeftMouseButton);

}

void AVRPawn::SelectReleased() {
    //Interaction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AVRPawn::GrabRightPressed() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarPressed();
			if (OverlapedRightParte) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarParteRight = false;
				bGrabRightParte = true;
				//if (OverlapedRightParte->Id != OverlapedLeftParte->IdParteRaiz) { //esta condicion no es necesaria
					//si no es la raiz de su jerarquia debo 
					//en relaidad no encesito este if, defrente tomar el IdParteRaiz y trabanr con esa parte
					//como hago en los demas, y da igual si
					OffsetRightParte = Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation() - MotionControllerRight->GetComponentLocation();//creo que esto no sera necesario
					//establecer la posicion y rotacion del punto de referenciay ponerlo de hijo de este control
					UE_LOG(LogClass, Log, TEXT("Calculando offset"));
					PuntoReferenciaRight->SetWorldLocation(Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation());
					PuntoReferenciaRight->SetWorldRotation(Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorRotation());
					UE_LOG(LogClass, Log, TEXT("Actualizando punto referencia"));

					//OverlapedRightParte->BuscarArticulacion();//en realidad esto deberia ser para todas las partes de la jerarquia, todo el tiempo, si es que esta en el modo de armado
					//si para toda la jerarquia aplicarlo recursivo quiza solo en las partes que tenga articulaciones libres
				//}
			}
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarPressed();
			Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			Interaction->PressPointerKey(EKeys::LeftMouseButton);
			//GrabRightTrasladarPressed();
			/*if (OverlapedRightParte) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarParteRight = false;
				bGrabRightParte = true;
				//if (OverlapedRightParte->Id != OverlapedLeftParte->IdParteRaiz) { //esta condicion no es necesaria
					//si no es la raiz de su jerarquia debo 
					//en relaidad no encesito este if, defrente tomar el IdParteRaiz y trabanr con esa parte
					//como hago en los demas, y da igual si
					OffsetRightParte = Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation() - MotionControllerRight->GetComponentLocation();//creo que esto no sera necesario
					//establecer la posicion y rotacion del punto de referenciay ponerlo de hijo de este control
					UE_LOG(LogClass, Log, TEXT("Calculando offset"));
					PuntoReferenciaRight->SetWorldLocation(Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation());
					PuntoReferenciaRight->SetWorldRotation(Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorRotation());
					UE_LOG(LogClass, Log, TEXT("Actualizando punto referencia"));

					//OverlapedRightParte->BuscarArticulacion();//en realidad esto deberia ser para todas las partes de la jerarquia, todo el tiempo, si es que esta en el modo de armado
					//si para toda la jerarquia aplicarlo recursivo quiza solo en las partes que tenga articulaciones libres
				//}
			}*/
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
			//GrabRightArmarPressed();
			if (OverlapedRightBloque) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarBloqueRight = false;
				bGrabRightBloque = true;
                UE_LOG(LogClass, Log, TEXT("Calculando offset"));
                PuntoReferenciaRight->SetWorldLocation(OverlapedRightBloque->GetActorLocation());
                PuntoReferenciaRight->SetWorldRotation(OverlapedRightBloque->GetActorRotation());
                UE_LOG(LogClass, Log, TEXT("Actualizando punto referencia"));
                OverlapedRightBloque->SeguirObjeto(PuntoReferenciaRight);
                if (bGrabLeftBloque && OverlapedRightBloque == OverlapedLeftBloque) {
                    OverlapedLeftBloque->SeguirObjetos(PuntoReferenciaLeft, PuntoReferenciaRight);
                    UE_LOG(LogClass, Log, TEXT("Enviado informacion de escalada"));
                }
			}
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::GrabRightTick() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarTick();
			if (bGrabRightParte && OverlapedRightParte) {
				Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->SetWorldLocation(PuntoReferenciaRight->GetComponentLocation());
				Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->SetWorldRotation(PuntoReferenciaRight->GetComponentRotation());
				UE_LOG(LogClass, Log, TEXT("Actualizando Poisicion jerarquia tick"));
				Jerarquias[OverlapedRightParte->IdParteRaiz]->ActualizarNodos();
				//Jerarquias[OverlapedRightParte->IdParteRaiz]->ActualizarPila();
			}
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarTick();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabRightTrasladarTick();
			/*if (bGrabRightParte && OverlapedRightParte) {
				Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->SetWorldLocation(PuntoReferenciaRight->GetComponentLocation());
				Jerarquias[OverlapedRightParte->IdParteRaiz]->Root->SetWorldRotation(PuntoReferenciaRight->GetComponentRotation());
				UE_LOG(LogClass, Log, TEXT("Actualizando Poisicion jerarquia tick"));
				Jerarquias[OverlapedRightParte->IdParteRaiz]->ActualizarNodos();
				//Jerarquias[OverlapedRightParte->IdParteRaiz]->ActualizarPila();
			}*/
            //el movimiento debe estar controlado, solo a la poryeccion del robot, por lo tanto esto deberia estar controlado dentro de robot
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoTick();
        }
        break;
    }
    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            //si estoy en la tarea de armar
            //esto solo si estoy buscando parte o no tengo parte agregada
            if (bBuscarBloqueRight) {
                float Distancia = 0.0f;
                float DistanciaMin = std::numeric_limits<float>::max();
                OverlapedRightBloque = nullptr;
                for (int i = 0; i < OverlapedRightBloques.Num(); i++) {
                    Distancia = (OverlapedRightBloques[i]->GetActorLocation() - ColisionControllerRight->GetComponentLocation()).Size();
                    if (Distancia < DistanciaMin) {
                        OverlapedRightBloque = OverlapedRightBloques[i];
                        DistanciaMin = Distancia;
                        //UE_LOG(LogClass, Log, TEXT("Nuevo cercano right"));
                    }
                }
            }
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::GrabRightReleased() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarReleased();
			//if (bGrabRightParte) {
			if (OverlapedRightParte) {
				bBuscarParteRight = true;

				//debo ver si la jerarquia esta colisionando con lago, yy si es asi unir esas partes colisionas, por lo tanto ya no es a nivel de parte, si no de jerarauia de la parte que estoy sujetadno
				//en realidad no importa, el que llama a la union son las partes, ya no vvrpawn o no
				//llamo a la funcon unir, si es que hay solapmaniento que se auno, no es encesario llamar a la ufucion articulacion sobrepuest, oesa otra jfuncion tambien debe ecnargarse del control de nodos
				Jerarquias[OverlapedRightParte->IdParteRaiz]->RealizarUniones();
				//como comprobar que la jerarqui ya esta completa??
				//como evitar que amboas partes, es decir que las dos articulaciones corresponidetes, solo una ejecutes su fucnioalidad de unir, para evitar errores?
				//quiza solo deberia agregarl el addynamic una de ellas y la otra no
				//despues de realizar las uniones, pues no se la pregunta es, que jerarquia se deshiczo esta o la otra?
				//cual esxiste,? debo cambiar?
				int JerarquiaCompleta = -1;
				for (int i = 0; i < Jerarquias.Num(); i++) {
					if (Jerarquias[i]->CantidadPartes == 10) {
						JerarquiaCompleta = i;
					}
				}
				if (JerarquiaCompleta != -1) {//hay alguna jerarquia completa
				//if (Jerarquia->AllNodesCreated()) {//estoes para cuando una de las jeraruias ya este complea, esta comrobacion la deberia hacer internamente la clase robot, ya no desde aquia, y solo pasar al modo rotacion

					TArray<AActor *> RobotsEncontrados;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobot::StaticClass(), RobotsEncontrados);
					//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
					for (int i = 0; i < RobotsEncontrados.Num(); i++) {
						//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
							//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
						//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
						ARobot * const RobotEncontrado = Cast<ARobot>(RobotsEncontrados[i]);
						Jerarquia = Jerarquias[JerarquiaCompleta];
						SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
						if (RobotEncontrado) {
							//este jerarauias es las mismas del robot?? revisarlo mañana si es que no aguntas hoy
							RobotEncontrado->Jerarquia = Jerarquias[JerarquiaCompleta];
                            Jerarquia->PilaCodigo = RobotEncontrado->PilaCodigo;
							RobotEncontrado->SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
                            //Jerarquia->SetActorLocation(FVector(24.0f, 104.0f, 30.0f));
                            //Jerarquia->SetActorRotation(FRotator(0.0f, -60.0f, 0.0f));
						}
					}
                    bBuscarParteLeft = true;
                    bGrabLeftParte = false;
				}
				bGrabRightParte = false;
			}
				//guardar offset de la parte
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarReleased();
			Interaction->ReleasePointerKey(EKeys::LeftMouseButton);
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabRightTrasladarReleased();
			Interaction->ReleasePointerKey(EKeys::LeftMouseButton);
			if (OverlapedRightParte) {
				bBuscarParteRight = true;
				//Jerarquias[OverlapedRightParte->IdParteRaiz]->RealizarUniones();
                //crear punto para traslacion
				bGrabRightParte = false;
			}
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoReleased();
        }
        break;
    }
    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            if (OverlapedRightBloque) {
                bBuscarBloqueRight = true;
                bGrabRightBloque = false;
                OverlapedRightBloque->NoSeguir();
            }
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::GrabLeftPressed() {//copia del derecho pero sin comentarios, y con los nombres invertidos
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarPressed();
			if (OverlapedLeftParte) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarParteLeft = false;
				bGrabLeftParte = true;
				//if (OverlapedRightParte->Id != OverlapedLeftParte->IdParteRaiz) { //esta condicion no es necesaria
					//si no es la raiz de su jerarquia debo 
					//en relaidad no encesito este if, defrente tomar el IdParteRaiz y trabanr con esa parte
					//como hago en los demas, y da igual si
					OffsetLeftParte = Jerarquias[OverlapedLeftParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation() - MotionControllerLeft->GetComponentLocation();//creo que esto no sera necesario
					//establecer la posicion y rotacion del punto de referenciay ponerlo de hijo de este control
					PuntoReferenciaLeft->SetWorldLocation(Jerarquias[OverlapedLeftParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation());
					PuntoReferenciaLeft->SetWorldRotation(Jerarquias[OverlapedLeftParte->IdParteRaiz]->Root->ParteAsociada->GetActorRotation());

					//OverlapedLeftParte->BuscarArticulacion();//en realidad esto deberia ser para todas las partes de la jerarquia, todo el tiempo, si es que esta en el modo de armado
					//toda la jerarauiq que busque colisiones
					//si para toda la jerarquia aplicarlo recursivo quiza solo en las partes que tenga articulaciones libres
				//}
			}
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {//se supone que en este modo la jerarquia ya esta lista, y no hay partes no unidas
			//GrabLeftRotarPressed();
			if (OverlapedLeftParte) {//codifo repetido, encapsularlo en una funcion
				bBuscarParteLeft = false;
				bGrabLeftParte = true;
				if (OverlapedLeftParte->bConectado) {//si la parte ya esta conectada eso quiere decir que la jerarrquia ya teiene raiz
					if (bGrabRightMuneco) {
						//rotar la pieza
					}
					else {//si no esta sijetado por la otra mano
						OffsetLeftParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerLeft->GetComponentLocation();
						bGrabLeftMuneco = true;
						PuntoReferenciaLeft->SetWorldLocation(Jerarquia->Root->ParteAsociada->GetActorLocation());
						PuntoReferenciaLeft->SetWorldRotation(Jerarquia->Root->ParteAsociada->GetActorRotation());
					}
				}
			}
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarPressed();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoPressed();
        }
        break;
    }

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
			//GrabRightArmarPressed();
			if (OverlapedLeftBloque) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarBloqueLeft = false;
				bGrabLeftBloque = true;
                UE_LOG(LogClass, Log, TEXT("Calculando offset"));
                PuntoReferenciaLeft->SetWorldLocation(OverlapedLeftBloque->GetActorLocation());
                PuntoReferenciaLeft->SetWorldRotation(OverlapedLeftBloque->GetActorRotation());
                UE_LOG(LogClass, Log, TEXT("Actualizando punto referencia"));
                OverlapedLeftBloque->SeguirObjeto(PuntoReferenciaLeft);
                if (bGrabRightBloque && OverlapedLeftBloque == OverlapedRightBloque) {//he sujetado el mismo bloque que mi mano derecha
                    OverlapedRightBloque->SeguirObjetos(PuntoReferenciaRight, PuntoReferenciaLeft);
                    UE_LOG(LogClass, Log, TEXT("Enviado informacion de escalada"));
                }
			}
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::GrabLeftTick() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarTick();
			if (bGrabLeftParte && OverlapedLeftParte) {
				Jerarquias[OverlapedLeftParte->IdParteRaiz]->Root->SetWorldLocation(PuntoReferenciaLeft->GetComponentLocation());
				Jerarquias[OverlapedLeftParte->IdParteRaiz]->Root->SetWorldRotation(PuntoReferenciaLeft->GetComponentRotation());
				Jerarquias[OverlapedLeftParte->IdParteRaiz]->ActualizarNodos();
				//Jerarquias[OverlapedLeftParte->IdParteRaiz]->ActualizarPila();
			}
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabLeftRotarTick();
			if (bGrabLeftParte) {
				//if (OverlapedLeftParte->bConectado) {
                //deberia tener la jerarquia final en la variable Jerarquia?
					if (Jerarquia->Root) {//bRootEstablecida
						Jerarquia->Root->SetWorldLocation(PuntoReferenciaLeft->GetComponentLocation());
						Jerarquia->Root->SetWorldRotation(PuntoReferenciaLeft->GetComponentRotation());
						Jerarquia->ActualizarNodos();
						Jerarquia->ActualizarPila();
					}
				//}
			}
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarTick();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoTick();
        }
        break;
    }

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            if (bBuscarBloqueLeft) {
                float Distancia;
                float DistanciaMin = std::numeric_limits<float>::max();
                OverlapedLeftBloque = nullptr;//de pronto no es necesario ponerlo null
                for (int i = 0; i < OverlapedLeftBloques.Num(); i++) {
                    Distancia = (OverlapedLeftBloques[i]->GetActorLocation() - ColisionControllerLeft->GetComponentLocation()).Size();
                    if (Distancia < DistanciaMin) {
                        OverlapedLeftBloque = OverlapedLeftBloques[i];
                        DistanciaMin = Distancia;
                        //UE_LOG(LogClass, Log, TEXT("Nuevo cercano left"));
                    }
                }
            }
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::GrabLeftReleased() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarReleased();
			if (OverlapedLeftParte) {
				bBuscarParteLeft = true;

				Jerarquias[OverlapedLeftParte->IdParteRaiz]->RealizarUniones();
				int JerarquiaCompleta = -1;
				for (int i = 0; i < Jerarquias.Num(); i++) {
					if (Jerarquias[i]->CantidadPartes == 10) {
						JerarquiaCompleta = i;
					}
				}
				if (JerarquiaCompleta != -1) {//hay alguna jerarquia completa
					TArray<AActor *> RobotsEncontrados;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobot::StaticClass(), RobotsEncontrados);
					//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
					for (int i = 0; i < RobotsEncontrados.Num(); i++) {
						//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
							//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
						//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
						ARobot * const RobotEncontrado = Cast<ARobot>(RobotsEncontrados[i]);
						Jerarquia = Jerarquias[JerarquiaCompleta];
						SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
						if (RobotEncontrado) {
							RobotEncontrado->SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
							//este jerarauias es las mismas del robot?? revisarlo mañana si es que no aguntas hoy
							RobotEncontrado->Jerarquia = Jerarquias[JerarquiaCompleta];
                            Jerarquia->PilaCodigo = RobotEncontrado->PilaCodigo;
						}
					}
                    bBuscarParteLeft = true;//para evitar errores por que aun se sujeta algo en el otro mando
                    bGrabLeftParte = false;
				}
				bGrabLeftParte = false;
			}

        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabLeftRotarReleased();
			if (OverlapedLeftParte) {
				bBuscarParteLeft = true;
				//guardar offset de la parte
				if (OverlapedLeftParte->bConectado) {
					//no hago nada por ahora
					if (bGrabRightMuneco) {
						//no se hace nada, jeje
					}
					else {
						bGrabLeftMuneco = false;
					}
				}
				bGrabLeftParte = false;//
			}
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarReleased();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoReleased();
        }
        break;
    }

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            if (OverlapedLeftBloque) {
                bBuscarBloqueLeft = true;
                bGrabLeftBloque = false;
                OverlapedLeftBloque->NoSeguir();
            }
        }
        break;
        case EVRCasaTask::EPlayTask: {
			//GrabRightRotarPressed();
			//Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void AVRPawn::OnBeginOverlapControllerRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (CurrentJerarquiaTask != EVRJerarquiaTask::ENoTask) {
        if (bBuscarParteRight) {//esto es para que mientras evitar el error de que cuando se esta trasladando el control y la parte, siempre detecta como si estuviera entrando en overlap en cada frame
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Parte"));
                        OverlapedRightParte = Parte;
                    }
                }

            }
        }
    }
    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueRight) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
                //ni comprobando
                ABloque * const Bloque = Cast<ABloque>(OtherActor);
                if (Bloque && !Bloque->IsPendingKill()) {
                    UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                    if (MeshBloque) {
                        //OverlapedRightBloque = Bloque;
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Bloque"));
                        OverlapedRightBloques.AddUnique(Bloque);
                    }
                }
            }
        }
    }
}

void AVRPawn::OnBeginOverlapControllerLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (CurrentJerarquiaTask != EVRJerarquiaTask::ENoTask) {
        if (bBuscarParteLeft) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Parte"));
                        OverlapedLeftParte = Parte;
                    }
                }

            }
        }
    }
    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueLeft) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
                //ni comprobando
                ABloque * const Bloque = Cast<ABloque>(OtherActor);
                if (Bloque && !Bloque->IsPendingKill()) {
                    UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                    if (MeshBloque) {
                        //OverlapedLeftBloque = Bloque;
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Bloque"));
                        OverlapedLeftBloques.AddUnique(Bloque);
                    }
                }
            }
        }
    }
}

void AVRPawn::OnEndOverlapControllerRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (CurrentJerarquiaTask != EVRJerarquiaTask::ENoTask) {
        if (bBuscarParteRight && !bGrabRightParte) {//esto es para que mientras evitar el error de que cuando se esta trasladando el control y la parte, siempre detecta como si estuviera entrando en overlap en cada frame
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill() && OverlapedRightParte == Parte) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlap Parte"));
                        OverlapedRightParte = nullptr;
                    }
                }
            }
        }
    }

    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueRight && !bGrabRightBloque) {
            ABloque * const Bloque = Cast<ABloque>(OtherActor);
            if (Bloque && !Bloque->IsPendingKill()) {
                UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);
                if(MeshBloque){
                    if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlap Bloque"));
                    //OverlapedRightBloque = nullptr;
                    OverlapedRightBloques.Remove(Bloque);
                }
            }
        }
    }
}

void AVRPawn::OnEndOverlapControllerLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (CurrentJerarquiaTask != EVRJerarquiaTask::ENoTask) {
        if (bBuscarParteLeft && !bGrabLeftParte) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlap Parte"));
                        OverlapedLeftParte = nullptr;
                    }
                }

            }
        }
    }

    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueLeft && !bGrabLeftBloque) {
            ABloque * const Bloque = Cast<ABloque>(OtherActor);
            if (Bloque && !Bloque->IsPendingKill()) {
                UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);
                if (MeshBloque) {
                    if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlap Bloque"));
                    //OverlapedLeftBloque = nullptr;
                    OverlapedLeftBloques.Remove(Bloque);
                }
            }
        }
    }
}

//debo mejorar el sistema de reconocimiento de partes, quiza manejaro en el tick en lugar de aqui, para quedarme con el mas cercano si hay varios

//si decido deguir con el el modelo de being y end, debo agregar el end, para qpue ponga en null, el OverlapedParte

//dos ideas para lo de la seleccion mas cercana
//la prrimera es usando las funcionde actules overlap, solo que en lugar de rremplazar el puntero, se introducen a un array, y se determina cual es el mas cercano entre ellos en todos los tick.
//hasta que presione seleccionar, ahi asigno recien al puntero y dejo de comprobar el mas cercano
//cuando una parte sale del la colision, se le saca del array

//la otra forma es llamar a la funicon geto overlaed actors en el tick todo el tiempo y obtener el array con los actores solpados, creo que esta seria innecesariamente lento
//al tener que llenar arryas todo el tiempo
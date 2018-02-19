// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Parte.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Robot.h"
#include "Jerarquia.h"
#include "Bloque.h"




// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

    MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
    MotionControllerRight->SetupAttachment(RootComponent);
    MotionControllerRight->SetRelativeLocation(FVector(50.0f, 40.0f, 0.0f));

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

    Velocidad = 200.0f;
    bPadDerecho = false;
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
    if (bPadDerecho && (MX != 0.0f || MY != 0.0f || MZ != 0.0f)) {
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

	/*if (bGrabRightParte) {
		OverlapedRightParte->SetActorLocation(MotionControllerRight->GetComponentLocation() + OffsetRightParte);
	}*/
	GrabRightTick();
	GrabLeftTick();

	/*if (bGrabLeftParte && RootParte) {
		FVector OffsetEliminar;
		if (RootParte->Hijos.Num()) {
			OffsetEliminar = RootParte->Hijos[0]->GetActorLocation() - RootParte->GetActorLocation();
		}
		RootParte->SetActorLocation(MotionControllerLeft->GetComponentLocation() + OffsetLeftParte);
		//esta traslacion se deberia aplicar a los hijos, por las trasnfomarciones y la jerarquia, por ahora hare una funcion sencilla para lo del hijo asumiendo que una dos cabezas
		if (RootParte->Hijos.Num()) {
			RootParte->Hijos[0]->SetActorLocation(MotionControllerLeft->GetComponentLocation() + OffsetLeftParte + OffsetEliminar);
		}
	}*/
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAction("Moverse", IE_Pressed, this, &AVRPawn::PadDerechoPressed);
    InputComponent->BindAction("Moverse", IE_Released, this, &AVRPawn::PadDerechoReleased);
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

    //es probable que no use el puntero laser, por lo tanto no existira select, con esa funcionalidad
    //pero si usare el triger para sostener objetos
}

void AVRPawn::CambiarLaser(int Indice) {
    if (Indice < Lasers.Num()) {
        LaserIndice = Indice;
        Laser->SetTemplate(Lasers[Indice]);
    }
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
}

EVRJerarquiaTask AVRPawn::GetJerarquiaTask() {
	return CurrentJerarquiaTask;
}

void AVRPawn::SetCasaTask(EVRCasaTask NewCasaTask) {
    CurrentCasaTask = NewCasaTask;
	//UE_LOG(LogClass, Log, TEXT("Cambiando Casa Task VRPawn %d"), NewCasaTask);
}

EVRCasaTask AVRPawn::GetCasaTask() {
    return CurrentCasaTask;
}

void AVRPawn::PadDerechoPressed() {
    bPadDerecho = true;
}

void AVRPawn::PadDerechoReleased() {
    bPadDerecho = false;
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
			//GrabRightTrasladarPressed();
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
            UE_LOG(LogClass, Log, TEXT("Armar"));
			//GrabRightArmarPressed();
			if (OverlapedRightBloque) {//encapsularlo en la funcion anterior para manter un orden
				bBuscarBloqueRight = false;
				bGrabRightBloque = true;
                UE_LOG(LogClass, Log, TEXT("Calculando offset"));
                PuntoReferenciaRight->SetWorldLocation(OverlapedRightBloque->GetActorLocation());
                PuntoReferenciaRight->SetWorldRotation(OverlapedRightBloque->GetActorRotation());
                UE_LOG(LogClass, Log, TEXT("Actualizando punto referencia"));
                OverlapedRightBloque->SeguirObjeto(PuntoReferenciaRight);
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
			//GrabRightArmarPressed();
			//if (bGrabRightBloque && OverlapedRightBloque) {
				//OverlapedRightBloque->SetActorLocation(PuntoReferenciaRight->GetComponentLocation());
				//OverlapedRightBloque->SetActorRotation(PuntoReferenciaRight->GetComponentRotation());
				//UE_LOG(LogClass, Log, TEXT("Actualizando Poisicion jerarquia tick"));
			//}
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
							RobotEncontrado->SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
							SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
							//este jerarauias es las mismas del robot?? revisarlo mañana si es que no aguntas hoy
							RobotEncontrado->Jerarquia = Jerarquias[JerarquiaCompleta];
						}
					}
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
			//GrabRightArmarPressed();
			//if (bGrabLeftBloque && OverlapedLeftBloque) {
				//OverlapedLeftBloque->SetActorLocation(PuntoReferenciaLeft->GetComponentLocation());
				//OverlapedLeftBloque->SetActorRotation(PuntoReferenciaLeft->GetComponentRotation());
				//UE_LOG(LogClass, Log, TEXT("Actualizando Poisicion jerarquia tick"));
			//}
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
							SetJerarquiaTask(EVRJerarquiaTask::ERotationTask);
							//este jerarauias es las mismas del robot?? revisarlo mañana si es que no aguntas hoy
							RobotEncontrado->Jerarquia = Jerarquias[JerarquiaCompleta];
						}
					}
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
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Parte"));
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
                        OverlapedRightParte = Parte;
                    }
                }

            }
        }
    }
    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueRight) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Bloque"));
                //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
                //ni comprobando
                ABloque * const Bloque = Cast<ABloque>(OtherActor);
                if (Bloque && !Bloque->IsPendingKill()) {
                    if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Bloque"));
                    UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                    if (MeshBloque) {
                        OverlapedRightBloque = Bloque;
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
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Parte"));
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
                        OverlapedLeftParte = Parte;
                    }
                }

            }
        }
    }
    if (CurrentCasaTask != EVRCasaTask::ENoTask) {
        if (bBuscarBloqueLeft) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Bloque"));
                //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
                //ni comprobando
                ABloque * const Bloque = Cast<ABloque>(OtherActor);
                if (Bloque && !Bloque->IsPendingKill()) {
                    if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Bloque"));
                    UStaticMeshComponent * const MeshBloque = Cast<UStaticMeshComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
                    if (MeshBloque) {
                        OverlapedLeftBloque = Bloque;
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
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("EndOverlap"));
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill() && OverlapedRightParte == Parte) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
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
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Bloque"));
                    OverlapedRightBloque= nullptr;
                }
            }
        }
    }
}

void AVRPawn::OnEndOverlapControllerLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if (CurrentJerarquiaTask != EVRJerarquiaTask::ENoTask) {
        if (bBuscarParteLeft && !bGrabLeftParte) {
            if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("EndOverlap"));
                AParte * const Parte = Cast<AParte>(OtherActor);
                if (Parte && !Parte->IsPendingKill()) {
                    UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
                    if (MeshParte) {
                        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
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
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Bloque"));
                    OverlapedLeftBloque = nullptr;
                }
            }
        }
    }
}

//debo mejorar el sistema de reconocimiento de partes, quiza manejaro en el tick en lugar de aqui, para quedarme con el mas cercano si hay varios

//si decido deguir con el el modelo de being y end, debo agregar el end, para qpue ponga en null, el OverlapedParte

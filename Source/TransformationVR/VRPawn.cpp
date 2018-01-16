// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Parte.h"
#include "Materials/Material.h"



// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
    VRCamera->SetupAttachment(RootComponent);

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

	ColisionControllerRight = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColisionControllerRightt"));
	ColisionControllerRight->SetupAttachment(MotionControllerRight);
	ColisionControllerRight->InitCapsuleSize(5.0f, 5.0f);
	ColisionControllerRight->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
	ColisionControllerRight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    ColisionControllerRight->OnComponentBeginOverlap.AddDynamic(this, &AVRPawn::OnBeginOverlapControllerRight);


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
    static ConstructorHelpers::FObjectFinder<UParticleSystem> EfectoImpactoAsset(TEXT("ParticleSystem'/Game/Visualization/ParticleSystems/LaserImpact/LaserImpactRotacion.LaserImpactRotacion'"));
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
    InputComponent->BindAction("Select", IE_Pressed, this, &AVRPawn::SelectPressed);
    InputComponent->BindAction("Select", IE_Released, this, &AVRPawn::SelectReleased);
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

void AVRPawn::PadDerechoPressed() {
    bPadDerecho = true;
}

void AVRPawn::PadDerechoReleased() {
    bPadDerecho = false;
}

void AVRPawn::SelectPressed() {
	//tomar como referencia el selecte del arbolito
}

void AVRPawn::SelectReleased() {
}

void AVRPawn::GrabRightPressed() {
	if (OverlapedRightParte) {
		bBuscarParteRight = false;
		bGrabRightParte = true;
		//guardar offset de la parte
		if (OverlapedRightParte->bConectado) {
			//deberia tener un if, si tuviera sujetado el muneco en  la otra mano, entonces quiere decir que estoy seleccionadno una parte para rotarla en respecto a asu articulacion
			//pero si no tengo el muneco en la otra mano, entonces debo hacer esto que estaba aqui antes, de sujetar al muneco y trasladara segun la raiz
			//OffsetRightParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerRight->GetComponentLocation();
			//bGrabRightMuneco = true;
			//este if podria provocar errores
			if (bGrabLeftMuneco) {
				//rotar la pieza
			}
			else {//si no tengo el muñeco en la otra mano
				OffsetRightParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerRight->GetComponentLocation();//creo que esto no sera necesario
				bGrabRightMuneco = true;
				//establecer la posicion y rotacion del punto de referenciay ponerlo de hijo de este control
				PuntoReferenciaRight->SetWorldLocation(Jerarquia->Root->ParteAsociada->GetActorLocation());
				PuntoReferenciaRight->SetWorldRotation(Jerarquia->Root->ParteAsociada->GetActorRotation());
				//PuntoReferenciaRobot->SetRelativeLocation(MotionControllerLeft->GetComponentTransform().TransformPosition(Jerarquia->Root->ParteAsociada->GetActorLocation()));
			}
		}
		else {//si no esta conectada
			if (Jerarquia->Root) {
				OffsetRightParte = OverlapedRightParte->GetActorLocation() - MotionControllerRight->GetComponentLocation();
				PuntoReferenciaRight->SetWorldLocation(OverlapedRightParte->GetActorLocation());
				PuntoReferenciaRight->SetWorldRotation(OverlapedRightParte->GetActorRotation());
				OverlapedRightParte->BuscarArticulacion();
				bGrabRightMuneco = false;
			}
			else {
				OffsetRightParte = OverlapedRightParte->GetActorLocation() - MotionControllerRight->GetComponentLocation();
				PuntoReferenciaRight->SetWorldLocation(OverlapedRightParte->GetActorLocation());
				PuntoReferenciaRight->SetWorldRotation(OverlapedRightParte->GetActorRotation());
				Jerarquia->Root = &(Jerarquia->TransformacionesPartes[OverlapedRightParte->Id]);
				Jerarquia->CrearNodo(Jerarquia->Root->ParteAsociada);
				Jerarquia->ActualizarNodos();
				Jerarquia->Layout();
				Jerarquia->AplicarLayout();
				OverlapedRightParte->bConectado = true;
				bRootEstablecida = true;
				bGrabRightMuneco = true;
			}
		}
	}
}

void AVRPawn::GrabRightTick() {
	if (bGrabRightParte) {
		if (OverlapedRightParte->bConectado) {
			if (Jerarquia->Root) {//bRootEstablecida
				//OffsetLeftParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerLeft->GetComponentLocation();//este offset no deberia calcularse solo una vez?
				//FVector Traslado = MotionControllerRight->GetComponentLocation() + OffsetRightParte - Jerarquia->Root->GetWorldLocation();

				//Jerarquia->Root->Trasladar(Traslado);
				//Jerarquia->Root->SetWorldLocation(MotionControllerRight->GetComponentLocation());
				Jerarquia->Root->SetWorldLocation(PuntoReferenciaRight->GetComponentLocation());
				Jerarquia->Root->SetWorldRotation(PuntoReferenciaRight->GetComponentRotation());
				//Jerarquia->Actualizar();
				//Jerarquia->Root->CalcularHDesdeHW();
				Jerarquia->ActualizarWorlds();
				Jerarquia->ActualizarPila();
			}
		}
		else {//si no esta conectada
			//OverlapedRightParte->SetActorLocation(MotionControllerRight->GetComponentLocation() + OffsetRightParte);
			OverlapedRightParte->SetActorLocation(PuntoReferenciaRight->GetComponentLocation());
			OverlapedRightParte->SetActorRotation(PuntoReferenciaRight->GetComponentRotation());
			Jerarquia->TransformacionesPartes[OverlapedRightParte->Id].ActualizarDesdeParte();
			//UE_LOG(LogClass, Log, TEXT("Trasladando Parte"));

		}
	}
}

void AVRPawn::GrabRightReleased() {
	if (OverlapedRightParte) {
		bBuscarParteRight = true;
		//guardar offset de la parte
		if (OverlapedRightParte->bConectado) {
			//no hago nada por ahora
			if (bGrabLeftMuneco) {
				//no se hace nada, jeje
			}
			else {
				bGrabRightMuneco = false;
			}
		}
		else {//si no esta conectada
			if (OverlapedRightParte->bArticulacionSobrepuesta) {//si hay una articualcion sobre puesta, debo unirla al munéco, cambiar un poco la posicion para juntar las articulaciones, y llamar a las funciones de las partes para inhabilitar las articulaciones unidas, e iniciar los calculos de las matrices de ser necesario
				//en teroai se supone que tengo un robot en mi mano izquierda por lo tanto deberia unirlo de frente sin embargo verifico
				if (bGrabLeftMuneco) {//si tengo sujeto en el otro control algo, manejarlo con un bool, en lugar de tener una parte root ahi, ya que no lo necesito
					//por ahor no hay jerarquia, y la union se debe hacer a la parte, por lo tanto la unicion la deberia hacer una funcion dentro de la parte que estoy uniendo, que se encargue de unirse a si misma al la jeraquia
					OverlapedRightParte->UnirConParteSobrepuesta();
					Jerarquia->UnirPadreHijo(OverlapedRightParte->OverlapedParte->Id, OverlapedRightParte->Id);
					Jerarquia->Layout();
					Jerarquia->AplicarLayout();
					Jerarquia->ActualizarPila();

					
				}
				//pero si no lo hbuiera sobre puesto a otra parte no unida, no deberia uniralas
			}
			OverlapedRightParte->NoBuscarArticulacion();
		}
		bGrabRightParte = false;
	}
}

void AVRPawn::GrabLeftPressed() {//copia del derecho pero sin comentarios, y con los nombres invertidos
	if (OverlapedLeftParte) {
		bBuscarParteLeft = false;
		bGrabLeftParte = true;
		if (OverlapedLeftParte->bConectado) {//si la parte ya esta conectada eso quiere decir que la jerarrquia ya teiene raiz
			if (bGrabRightMuneco) {
				//rotar la pieza
			}
			else {
				OffsetLeftParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerLeft->GetComponentLocation();
				bGrabLeftMuneco = true;
				PuntoReferenciaLeft->SetWorldLocation(Jerarquia->Root->ParteAsociada->GetActorLocation());
				PuntoReferenciaLeft->SetWorldRotation(Jerarquia->Root->ParteAsociada->GetActorRotation());
			}
		}
		else {//si no esta conectada
			if (Jerarquia->Root) {
				OffsetLeftParte = OverlapedLeftParte->GetActorLocation() - MotionControllerLeft->GetComponentLocation();
				PuntoReferenciaLeft->SetWorldLocation(OverlapedLeftParte->GetActorLocation());
				PuntoReferenciaLeft->SetWorldRotation(OverlapedLeftParte->GetActorRotation());
				OverlapedLeftParte->BuscarArticulacion();
				bGrabLeftMuneco = false;
			}
			else {
				OffsetLeftParte = OverlapedLeftParte->GetActorLocation() - MotionControllerLeft->GetComponentLocation();
				PuntoReferenciaLeft->SetWorldLocation(OverlapedLeftParte->GetActorLocation());
				PuntoReferenciaLeft->SetWorldRotation(OverlapedLeftParte->GetActorRotation());
				Jerarquia->Root = &(Jerarquia->TransformacionesPartes[OverlapedLeftParte->Id]);
				Jerarquia->CrearNodo(Jerarquia->Root->ParteAsociada);
				Jerarquia->ActualizarNodos();
				Jerarquia->Layout();
				Jerarquia->AplicarLayout();
				OverlapedLeftParte->bConectado = true;
				bRootEstablecida = true;
				bGrabLeftMuneco = true;
			}
		}
	}
}

void AVRPawn::GrabLeftTick() {
	if (bGrabLeftParte) {
		if (OverlapedLeftParte->bConectado) {
			if (Jerarquia->Root) {//bRootEstablecida
				//OffsetLeftParte = Jerarquia->Root->ParteAsociada->GetActorLocation() - MotionControllerLeft->GetComponentLocation();//este offset no deberia calcularse solo una vez?
				//FVector Traslado = MotionControllerLeft->GetComponentLocation() + OffsetLeftParte - Jerarquia->Root->GetWorldLocation();

				//Jerarquia->Root->Trasladar(Traslado);
				//Jerarquia->TraslacionTemporal = Traslado;
				//Jerarquia->Actualizar();
				//Jerarquia->Root->CalcularHDesdeHW();

				Jerarquia->Root->SetWorldLocation(PuntoReferenciaLeft->GetComponentLocation());
				Jerarquia->Root->SetWorldRotation(PuntoReferenciaLeft->GetComponentRotation());
				Jerarquia->ActualizarWorlds();//probar!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				Jerarquia->ActualizarPila();
			}
		}
		else {//si no esta conectada
			//OverlapedLeftParte->SetActorLocation(MotionControllerLeft->GetComponentLocation() + OffsetLeftParte);
			OverlapedLeftParte->SetActorLocation(PuntoReferenciaLeft->GetComponentLocation());
			OverlapedLeftParte->SetActorRotation(PuntoReferenciaLeft->GetComponentRotation());
			Jerarquia->TransformacionesPartes[OverlapedLeftParte->Id].ActualizarDesdeParte();
		}
	}
}

void AVRPawn::GrabLeftReleased() {
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
		else {//si no esta conectada
			if (OverlapedLeftParte->bArticulacionSobrepuesta) {//si hay una articualcion sobre puesta, debo unirla al munéco, cambiar un poco la posicion para juntar las articulaciones, y llamar a las funciones de las partes para inhabilitar las articulaciones unidas, e iniciar los calculos de las matrices de ser necesario
				//en teroai se supone que tengo un robot en mi mano izquierda por lo tanto deberia unirlo de frente sin embargo verifico
				if (bGrabRightMuneco) {//si tengo sujeto en el otro control algo, manejarlo con un bool, en lugar de tener una parte root ahi, ya que no lo necesito
					//por ahor no hay jerarquia, y la union se debe hacer a la parte, por lo tanto la unicion la deberia hacer una funcion dentro de la parte que estoy uniendo, que se encargue de unirse a si misma al la jeraquia
					OverlapedLeftParte->UnirConParteSobrepuesta();
					Jerarquia->UnirPadreHijo(OverlapedLeftParte->OverlapedParte->Id, OverlapedLeftParte->Id);
					Jerarquia->Layout();
					Jerarquia->AplicarLayout();
					Jerarquia->ActualizarPila();
					
				}
				//pero si no lo hbuiera sobre puesto a otra parte no unida, no deberia uniralas
			}
			OverlapedLeftParte->NoBuscarArticulacion();
		}
		bGrabLeftParte = false;//deberia usar este bool en lugar del puntero en el if al inicio??
	}
}

void AVRPawn::OnBeginOverlapControllerRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if(bBuscarParteRight){//esto es para que mientras evitar el error de que cuando se esta trasladando el control y la parte, siempre detecta como si estuviera entrando en overlap en cada frame
		if ( (OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
			if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap"));
			AParte * const Parte = Cast<AParte>(OtherActor);
			if (Parte && !Parte->IsPendingKill()) {
				UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
				if(MeshParte){
					if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
					OverlapedRightParte = Parte;
				}
			}
		}
	}
}

void AVRPawn::OnBeginOverlapControllerLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (bBuscarParteLeft) {
		if ( (OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
			if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap"));
			AParte * const Parte = Cast<AParte>(OtherActor);
			if (Parte && !Parte->IsPendingKill()) {
				UStaticMeshComponent * const MeshParte = Cast<UStaticMeshComponent>(OtherComp);
				if(MeshParte){
					if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Parte"));
					OverlapedLeftParte = Parte;
				}
			}
		}
	}
}

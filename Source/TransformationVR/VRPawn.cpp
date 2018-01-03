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


    Velocidad = 200.0f;
    bPadDerecho = false;
    LaserIndice = 0;

	bGrabRightParte = false;
	bGrabLeftParte = false;
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

	if (bGrabRightParte) {
		OverlapedRightParte->SetActorLocation(MotionControllerRight->GetComponentLocation() + OffsetRightParte);
	}

	if (bGrabLeftParte) {
		FVector OffsetEliminar;
		if (RootParte->Hijos.Num()) {
			OffsetEliminar = RootParte->Hijos[0]->GetActorLocation() - RootParte->GetActorLocation();
		}
		RootParte->SetActorLocation(MotionControllerLeft->GetComponentLocation() + OffsetLeftParte);
		//esta traslacion se deberia aplicar a los hijos, por las trasnfomarciones y la jerarquia, por ahora hare una funcion sencilla para lo del hijo asumiendo que una dos cabezas
		if (RootParte->Hijos.Num()) {
			RootParte->Hijos[0]->SetActorLocation(MotionControllerLeft->GetComponentLocation() + OffsetLeftParte + OffsetEliminar);
		}
	}
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
		bGrabRightParte = true;
		//guardar offset de la parte
		OffsetRightParte = OverlapedRightParte->GetActorLocation() - MotionControllerRight->GetComponentLocation();
		//en realidad deberia ser un offset relativo al motion controller, y luego cuando se porceda a asiganar a la paoscion de la parte se debe convertir al espacion global
	}
}

void AVRPawn::GrabRightReleased() {
	if (OverlapedRightParte) {
		bGrabRightParte = false;
	}
}

void AVRPawn::GrabLeftPressed() {
	if (OverlapedLeftParte) {
		bGrabLeftParte = true;
		if (bRootEstablecida) {
			//solo calcular el ofset de la raiz al control
			//estas traslaciones naturlamente aplican con las tranformaciones
		}
		else {
			RootParte = OverlapedLeftParte;
			bRootEstablecida = true;
			//como no hay raiz, establecer una automaticamente a partir de la parte que ha sido sostenida
			//esto afectara las transformaciones en generale, recalculando los valores usando la posicion global
		}
		//guardar offset de la parte
		OffsetLeftParte = RootParte->GetActorLocation() - MotionControllerLeft->GetComponentLocation();
		//en realidad deberia ser un offset relativo al motion controller, y luego cuando se porceda a asiganar a la paoscion de la parte se debe convertir al espacion global
	}
}

void AVRPawn::GrabLeftReleased() {
	if (OverlapedLeftParte) {
		bGrabLeftParte = false;
	}
}

void AVRPawn::OnBeginOverlapControllerRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
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
                //Animal->RecibirAtaque(Poder, GetActorLocation());
            }
        }
    }
}

void AVRPawn::OnBeginOverlapControllerLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
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
                //Animal->RecibirAtaque(Poder, GetActorLocation());
            }
        }
    }
}

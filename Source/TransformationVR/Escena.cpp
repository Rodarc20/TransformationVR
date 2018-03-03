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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlechaAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/FlechaGrande.FlechaGrande'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (FlechaAsset.Succeeded()) {
        FlechaX->SetStaticMesh(FlechaAsset.Object);
        FlechaY->SetStaticMesh(FlechaAsset.Object);
        FlechaZ->SetStaticMesh(FlechaAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;
            FlechaX->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
			//EjeYNormal = FlechaYMaterialAsset.Object;
            FlechaY->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
			//EjeZNormal = FlechaZMaterialAsset.Object;
            FlechaZ->SetMaterial(0, FlechaZMaterialAsset.Object);
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

    //TWidget->OcultarWidget();

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

void AEscena::GrabRightPressed() {
}

void AEscena::GrabRightTick() {
}

void AEscena::GrabRightReleased() {
    if (OverlapedRightEjes.Num() && Casa) {
        Casa->SetTransformacionEje(OverlapedRightEjes[0]);
    }
}

void AEscena::GrabLeftPressed() {
}

void AEscena::GrabLeftTick() {
}

void AEscena::GrabLeftReleased() {
    if (OverlapedLeftEjes.Num() && Casa) {
        Casa->SetTransformacionEje(OverlapedLeftEjes[0]);
    }
}

void AEscena::MostrarWidget() {
	Origen->SetVisibility(true);
	FlechaX->SetVisibility(true);
	FlechaY->SetVisibility(true);
	FlechaZ->SetVisibility(true);
}

void AEscena::OcultarWidget() {
	Origen->SetVisibility(false);
	FlechaX->SetVisibility(false);
	FlechaY->SetVisibility(false);
	FlechaZ->SetVisibility(false);
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
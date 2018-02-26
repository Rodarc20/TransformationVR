// Fill out your copyright notice in the Description page of Project Settings.

#include "Casa.h"
#include "Engine/Engine.h"
#include "VRPawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Bloque.h"


// Sets default values
ACasa::ACasa()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Zona = CreateDefaultSubobject<UBoxComponent>(TEXT("Zona"));
    RootComponent = Zona;
    Zona->InitBoxExtent(FVector(100.0f));
    Zona->OnComponentBeginOverlap.AddDynamic(this, &ACasa::OnBeginOverlapZona);
    Zona->OnComponentEndOverlap.AddDynamic(this, &ACasa::OnEndOverlapZona);

    bMoviendo = false;
    Velocidad = 20.0f;
    PosicionFlotando = FVector(0.0f, -380.0f, 40.0f);
    PosicionDescanso = FVector(0.0f, -380.0f, 0.0f);

    CurrentCasaTask = EVRCasaTask::ENoTask;
    LastCasaTask = EVRCasaTask::EArmarTask;

    CurrentTransformacionTarea = ETransformacionTarea::ENone;
    CurrentTransformacionEje = ETransformacionEje::ENone;
}

// Called when the game starts or when spawned
void ACasa::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor *> BloquesEncontrados;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloque::StaticClass(), BloquesEncontrados);
	//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
	for (int i = 0; i < BloquesEncontrados.Num(); i++) {
		//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
		//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
		ABloque * const BloqueEncontrado = Cast<ABloque>(BloquesEncontrados[i]);
        if (BloqueEncontrado) {
            //BloqueEncontrado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
            //supongamos que ya estan dentro de la jerarquia
            Bloques.Add(BloqueEncontrado);
        }
	}
	
    AVRPawn * MyVRPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (MyVRPawn) {
        Usuario = MyVRPawn;
    }
}

// Called every frame
void ACasa::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
    //no deberia comprobar todo el tiempo si esta armado o no, solo deberia ser cuando he soltado una parte, o que esta clase se comunique con sus partes

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            if (CasaArmada()) {
                SetCasaTask(EVRCasaTask::EPlayTask);
                //tambien deberia darle al usuario esto, como en la clase robot
            }
        }
        break;
        case EVRCasaTask::EPlayTask: {
            PlayTaskTick();
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
        }
        break;
    }

    if (bMoviendo) {
        //esto solo deberia poner el booleano a mobiendo
        FVector direccion = PosicionObjetivo - GetActorLocation();
        if (direccion.IsNearlyZero(0.05f)) {//mejorar esa presicion
            bMoviendo = false;
            SetActorLocation(PosicionObjetivo);
            UE_LOG(LogClass, Log, TEXT("Termino el trasladao"));
        }
        else {
            SetActorLocation(GetActorLocation() + (direccion.GetSafeNormal() * Velocidad * DeltaTime));
        }
    }


}

bool ACasa::CasaArmada() {
    bool res = true;
    for (int i = 0; i < Bloques.Num() && res; i++) {
        res = res & Bloques[i]->bArmado;
    }
    return res;
}

void ACasa::SetCasaTask(EVRCasaTask NewCasaTask) {
    CurrentCasaTask = NewCasaTask;
    //debo incaiar ciertas operaciones cuando inicie la ptarea play, como flotar la casa

    switch (CurrentCasaTask) {
        case EVRCasaTask::EArmarTask: {
            //activar efectos visuales necesarios
        }
        break;
        case EVRCasaTask::EPlayTask: {
            Flotar();
            //activar efectos visaules necesarios
        }
        break;
        default:
        case EVRCasaTask::ENoTask: {
            //Descender, si el usuario no esta que se desciendan, y se cambie a esta tarea, 
            //necesito un last current scene, para recordar en la que estaba si armando, o estaba en rotando, para que cuando salada la pase a none, y cuando me acerque de nuevo sepa en cual estaba y cambia a esa
            //descativar efectos visuales
            //hacer aterrizar la casa de ser necesario, al punto 0, de pronto no
            Aterrizar();
        }
        break;
    }
}

EVRCasaTask ACasa::GetCasaTask() {
    return CurrentCasaTask;
}

void ACasa::Flotar() {
    UE_LOG(LogClass, Log, TEXT("Flotando"));
    PosicionObjetivo = PosicionFlotando;
    bMoviendo = true;
}

void ACasa::Aterrizar() {
    UE_LOG(LogClass, Log, TEXT("Aterrizando"));
    PosicionObjetivo = PosicionDescanso;
    bMoviendo = true;
}

void ACasa::OnBeginOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Zona"));
        //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
        //ni comprobando
        AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
        if (VRPawn && !VRPawn->IsPendingKill()) {
            if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("VRPawn"));
            USphereComponent * const ColisionHead = Cast<USphereComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
            if (ColisionHead) {
                SetCasaTask(LastCasaTask);
                VRPawn->SetCasaTask(LastCasaTask);
                //VRPawn->SetCasaTask(CurrentCasaTask);
            }
        }
    }
}

void ACasa::OnEndOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
        if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Zona"));
        //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
        //ni comprobando
        AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
        if (VRPawn && !VRPawn->IsPendingKill()) {
            if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("VRPawn"));
            USphereComponent * const ColisionHead = Cast<USphereComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
            if (ColisionHead) {
                LastCasaTask = CurrentCasaTask;
                SetCasaTask(EVRCasaTask::ENoTask);
                VRPawn->SetCasaTask(EVRCasaTask::ENoTask);
                //VRPawn->SetCasaTask(EVRCasaTask::ENoTask);
            }
        }
    }
}

void ACasa::PlayTaskTick() {
    switch (CurrentTransformacionTarea) {
        case ETransformacionTarea::ETrasladar: {
            switch (CurrentTransformacionEje) {
                case ETransformacionEje::EEjeX: {
                }
                break;
                case ETransformacionEje::EEjeY: {
                }
                break;
                case ETransformacionEje::EEjeZ: {
                }
                break;
                default:
                case ETransformacionEje::ENone: {
                }
                break;
            }
        }
        break;
        case ETransformacionTarea::ERotar: {
            switch (CurrentTransformacionEje) {
                case ETransformacionEje::EEjeX: {
                }
                break;
                case ETransformacionEje::EEjeY: {
                }
                break;
                case ETransformacionEje::EEjeZ: {
                }
                break;
                default:
                case ETransformacionEje::ENone: {
                }
                break;
            }
        }
        break;
        case ETransformacionTarea::EEscalar: {
            switch (CurrentTransformacionEje) {
                case ETransformacionEje::EEjeX: {
                }
                break;
                case ETransformacionEje::EEjeY: {
                }
                break;
                case ETransformacionEje::EEjeZ: {
                }
                break;
                default:
                case ETransformacionEje::ENone: {
                }
                break;
            }
        }
        break;
        default:
        case ETransformacionTarea::ENone: {
        }
        break;
    }
}

void ACasa::SetTransformacionTarea(ETransformacionTarea NewTransformacionTarea) {
    CurrentTransformacionTarea = NewTransformacionTarea;
    //debo hacer ciertas cosas, como reestablecer valores tal vez, o botones fisicos
}

ETransformacionTarea ACasa::GetTransformacionTarea() {
    return CurrentTransformacionTarea;
}

void ACasa::SetTransformacionEje(ETransformacionEje NewTransformacionEje) {
    CurrentTransformacionEje = NewTransformacionEje;
}

ETransformacionEje ACasa::GetTransformacionEje() {
    return CurrentTransformacionEje;
}


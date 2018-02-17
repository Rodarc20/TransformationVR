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

    CurrentCasaTask = EVRCasaTask::EArmarTask;
}

// Called when the game starts or when spawned
void ACasa::BeginPlay()
{
	Super::BeginPlay();

	/*TArray<AActor *> BloquesEncontrados;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloque::StaticClass(), BloquesEncontrados);
	//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
	for (int i = 0; i < BloquesEncontrados.Num(); i++) {
		//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
		//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
		ABloque * const BloqueEncontrado = Cast<ABloque>(BloquesEncontrados[i]);
        if (BloqueEncontrado) {
            BloqueEncontrado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
        }
	}*/
	
}

// Called every frame
void ACasa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
                VRPawn->SetCasaTask(CurrentCasaTask);
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
                VRPawn->SetCasaTask(EVRCasaTask::ENoTask);
            }
        }
    }
}


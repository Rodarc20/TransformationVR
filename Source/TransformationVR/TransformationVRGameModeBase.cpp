// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformationVRGameModeBase.h"
#include "VRPawn.h"
#include "Parte.h"
#include "Jerarquia.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Public/UObject/ConstructorHelpers.h"


ATransformationVRGameModeBase::ATransformationVRGameModeBase() {
    PrimaryActorTick.bCanEverTick = true;//esto hacia falta en el tutprial, sin esto unreal no llama a la funcion tick en cada cuadro

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("Class'/Script/TransformationVR.VRPawn'"));
	if (PlayerPawnClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}

void ATransformationVRGameModeBase::BeginPlay() {

	Partes.SetNum(10);

	TArray<AActor *> PartesEncontradas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParte::StaticClass(), PartesEncontradas);
	//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
	for (int i = 0; i < PartesEncontradas.Num(); i++) {
		//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
		//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
		AParte * const ParteEncontrada = Cast<AParte>(PartesEncontradas[i]);
		if(ParteEncontrada)
			Partes[ParteEncontrada->Id] = ParteEncontrada;
	}

	//Instanciando jerarquia
	UWorld * const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		Jerarquia = World->SpawnActor<AJerarquia>(AJerarquia::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	for (int i = 0; i < 10 && i < Partes.Num(); i++) {//asociando partes a la jerarquia
		Jerarquia->TransformacionesPartes[i].ParteAsociada = Partes[i];
	}
    AVRPawn * MyVRPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MyVRPawn) {
		MyVRPawn->Jerarquia = Jerarquia;
	}
}

void ATransformationVRGameModeBase::Tick(float DeltaTime) {
}

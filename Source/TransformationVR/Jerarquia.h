// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Transformacion.h"
#include "Nodo.h"
#include "Parte.h"
#include "PilaOpenGL.h"
#include "MotionControllerComponent.h"
#include "Jerarquia.generated.h"


UCLASS()
class TRANSFORMATIONVR_API AJerarquia : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJerarquia();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualization - Referencias")
    //UMotionControllerComponent* RightController;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualization - Referencias")
    //UMotionControllerComponent* LeftController;

	TArray<Transformacion * > TransformacionesPartesPunteros;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	TArray<ANodo *> Nodos;

	Transformacion * Root;

	void UnirPadreHijo(int IdPadre, int IdHijo);

	void Actualizar();//actualiza toda la jerarquia

	void ActualizarWorlds();

	void CrearNodo(AParte * ParteAsociada);
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jerarquia")
    TSubclassOf<class ANodo> TipoNodo;//esto no es practio llenarlo en el cosntructor, cuando esta clase pase a bluprint sera mejor

    void ImprimirMatriz(FMatrix m);

	void ImprimirTransformacion(Transformacion * T);

	void Imprimir();

	void ImprimirMatrices(Transformacion * T);

	FVector TraslacionTemporal;//temporalmente usare esta variapble para traslador en funcion de este vector todos los elementso de la jerarquia, pero debo eliminar por que con esto no se trasladara todo

	void AplicarLayout();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void EjecutarAnimacion(int IdParte);

	void EjecutarAnimacionTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void ActualizarNodos();

	void Calculos(Transformacion * V);

	void Calculos2();

	void Calc();

	void Layout();

	float AnchoNodos;

	float AltoNodos;

	float DeltaNiveles;

	float DeltaHermanos;

	APilaOpenGL * PilaCodigo;

	int NumeroIdentaciones;

	FString Texto(Transformacion * T);

	FString Identacion(int Tam);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void ActualizarPila();

	float DistanciaLaserMaxima;

	void EstablecerRotacionEjeX(int IdParte, float angle);

	void EstablecerRotacionEjeY(int IdParte, float angle);

	void EstablecerRotacionEjeZ(int IdParte, float angle);

	void EstablecerRotacionEje(int IdParte, float angle, ETransformacionEje EjeRotacion);

    UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void ConfirmarRotacion(int IdParte, int IdRotacion);

    UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void CancelarRotacion(int IdParte, int IdRotacion);

    UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    AParte * GetParte(int IdParte);
	
	bool AllNodesCreated();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void AbsorberJerarquia(AJerarquia * Otra, int IdPadre, int IdHijo);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void ActualizarIdRaizParte(int NuevoIdRaizParte);

	bool ArticulacionSobrepuesta();

	bool RealizarUniones();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	int CantidadPartes;
	
};

//esta jerarquia tendra asociada a todas las partes, por lo tanto puede tener un arreglo de trasnforms del tamaño del numero de las partes
//luego el game mode, la instanciar esta objeto, els asigana a estos trasnforma las partes correspondientes con un id, los cuales ua estan fijos en las clases
//luego el vrpawn al interacutar con las partes, envia la informacion pertiniente a esta clase jerarquia, es decir el ide de la parte movida o unida , etc y los valores aporpiados para que esta jerarquia actualice
//
//a diferencia de la conexxion entre las partes la jerarquia no necesita poner los hijos en orden,
//solo poner padre e hijos, sin ditincion algunas:w

UENUM(BlueprintType)
enum class EVRJerarquiaTask : uint8 {
    ETraslationTask UMETA(DisplayName = "Trasladar"),
    ERotationTask UMETA(DisplayName = "Rotar"),
    EScaleTask UMETA(DisplayName = "Escalar"),
    EArmarTask UMETA(DisplayName = "Armar"),
    EPlayTask UMETA(DisplayName = "Play"),
    ENoTask UMETA(DisplayName = "Ninguno")
};


//la jeraarquia actuara como las visaulizacion es en el arbolito, detectando las cosas del puntero, y todo lo demas
//necestio conexion con el usuario, aun que tambien esot lo podria hacer el game mode, pero mejor lo dejo asi

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Transformacion.h"
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

	TArray<Transformacion> TransformacionesPartes;

	Transformacion * Root;

    FMatrix MatrizTraslacion(float x, float y, float z);

    FMatrix MatrizRotacionX(float angle);

    FMatrix MatrizRotacionY(float angle);

    FMatrix MatrizRotacionZ(float angle);

    FMatrix MultiplicacionMatriz(FMatrix a, FMatrix b);

	void Actualizar();//actualiza toda la jerarquia

    void ImprimirMatriz(FMatrix m);
	
	
	
};

//esta jerarquia tendra asociada a todas las partes, por lo tanto puede tener un arreglo de trasnforms del tama�o del numero de las partes
//luego el game mode, la instanciar esta objeto, els asigana a estos trasnforma las partes correspondientes con un id, los cuales ua estan fijos en las clases
//luego el vrpawn al interacutar con las partes, envia la informacion pertiniente a esta clase jerarquia, es decir el ide de la parte movida o unida , etc y los valores aporpiados para que esta jerarquia actualice
//
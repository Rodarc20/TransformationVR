// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"

/**
 * 
 */
class TRANSFORMATIONVR_API Transformacion
{
public:
	FMatrix H;
	FMatrix HW;//cuando este moviendo las partes, estare modificando esta matriz, para luego calcular la matriz local H
	Transformacion * Padre;
	TArray<Transformacion *> Hijos;

	AParte * ParteAsociada;
	void ActualizarParte();
	void ActualizarDesdeParte();

	//Solo para modificar las matrices, no para actulaizar los valores de las partes
	void SetLocation(FVector Posicion);//esta funcion estblece la posicion local, y actualiza la posicion global calculandola con la informacion del padre
	void SetWorldLocation(FVector Posicion);
	void SetRotation(FRotator Rotacion);
	void SetWorldRotation(FRotator Rotacion);
	FVector GetLocation();
	FVector GetWorldLocation();
	FRotator GetRotation();
	FRotator GetWorldRotation();
	void Trasladar(FVector Traslacion);//realiz las operacions para trasladar este transform
	void Rotar(FRotator Rotacion);//realiza las operaciones para trasladar este transforma

	FMatrix HLocal();//retorna el H respecto al padre
	FMatrix HWorld();//retorna el H respecto al origen del mundo
	void CalcularHWorld();
	void CalcularHWDesdeH();
	void CalcularHDesdeHW();
	FMatrix FromWorldToLocal(FMatrix NewHW);
	FMatrix FromLocalToWorld(FMatrix NewH);
	FMatrix MatrizTraslacion(float x, float y, float z);
	FMatrix MatrizRotacionX(float angle);
	FMatrix MatrizRotacionY(float angle);
	FMatrix MatrizRotacionZ(float angle);
	FMatrix MultiplicacionMatriz(FMatrix a, FMatrix b);

	//para el layout
	float Hojas;
	int Altura;
	int Nivel;
	FVector PosicionNodo;
	float WTam;
	float WInicio;


	Transformacion();
	~Transformacion();
};
//lo normal es que esto se usado en las partes para encontrar su ubicacione en el espacio, por fines deidacticos sera al reves

//necesito que tenga algunas operaciones y referencias a las transformaciones del padre y del hijo, solo por si a caso
//estos seran los nodos de la jerarquia, ya que podria tener multiples jeraquias, cada una con un trasform diferente para cada parte
//se que esto va en contra de una buena paractica, o que podria ser mas conveniente que cada parte tenga un transformacion, pero l=por las necesidades didacticas, sera asi.
//cada uno de estos trasnforma tendra la referencia a una parte.
//el gamemode, debera generar jerarquias creado estos nodoos transformacion con las aprtes ubicadas en el juego.
//o instanciandolas/
//gamemode, siempre debera actualizar esta jerarquia, o que se actualice a si misma, el pawn al mover las partes genera cambios en sus posiciones, globales, o locales, las cuales pasaran a recalcularse en la jerarquia, y actualizar el otro valor local o global
//esa cactualizacion o bien la hace gamemode, o bien la hace la jerarquia
//el flujo es , pawn mueve la parte, la parte actualiza su transform, la jeraquia recalcula todos los transforma del arbol, y resposiciona todas las partes del arbol calculando primero sus posiciones globales.
//quiza el reposicionamiento deba ahcerse si hubiera alguncambio, por ejemplo, que solo afecte a los hijos del trasnforma movido.:w



//voya requerir hacer los dos enfoques, de calcular el H en funcion del Hw y calcular el H en funcion del HW, dependiendo de cual sea el flujo depende la llamada a determindada funcionalidad
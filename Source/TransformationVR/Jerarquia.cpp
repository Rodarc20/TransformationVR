// Fill out your copyright notice in the Description page of Project Settings.

#include "Jerarquia.h"


// Sets default values
AJerarquia::AJerarquia()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJerarquia::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJerarquia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FMatrix AJerarquia::MatrizTraslacion(float x, float y, float z) {
    FMatrix Traslacion;
    Traslacion.M[0][0] = 1;
    Traslacion.M[0][1] = 0;
    Traslacion.M[0][2] = 0;
    Traslacion.M[0][3] = x;
    Traslacion.M[1][0] = 0;
    Traslacion.M[1][1] = 1;
    Traslacion.M[1][2] = 0;
    Traslacion.M[1][3] = y;
    Traslacion.M[2][0] = 0;
    Traslacion.M[2][1] = 0;
    Traslacion.M[2][2] = 1;
    Traslacion.M[2][3] = z;
    Traslacion.M[3][0] = 0;
    Traslacion.M[3][1] = 0;
    Traslacion.M[3][2] = 0;
    Traslacion.M[3][3] = 1;
    return Traslacion;
}

FMatrix AJerarquia::MatrizRotacionX(float angle) {
    FMatrix RotX;
    RotX.M[0][0] = cos(angle);
    RotX.M[0][1] = 0;
    RotX.M[0][2] = sin(angle);
    RotX.M[0][3] = 0;
    RotX.M[1][0] = 0;
    RotX.M[1][1] = 1;
    RotX.M[1][2] = 0;
    RotX.M[1][3] = 0;
    RotX.M[2][0] = -sin(angle);
    RotX.M[2][1] = 0;
    RotX.M[2][2] = cos(angle);
    RotX.M[2][3] = 0;
    RotX.M[3][0] = 0;
    RotX.M[3][1] = 0;
    RotX.M[3][2] = 0;
    RotX.M[3][3] = 1;
    return RotX;
}

FMatrix AJerarquia::MatrizRotacionY(float angle) {
    FMatrix RotY;
    RotY.M[0][0] = 1;
    RotY.M[0][1] = 0;
    RotY.M[0][2] = 0;
    RotY.M[0][3] = 0;
    RotY.M[1][0] = 0;
    RotY.M[1][1] = cos(angle);
    RotY.M[1][2] = -sin(angle);
    RotY.M[1][3] = 0;
    RotY.M[2][0] = 0;
    RotY.M[2][1] = sin(angle);
    RotY.M[2][2] = cos(angle);
    RotY.M[2][3] = 0;
    RotY.M[3][0] = 0;
    RotY.M[3][1] = 0;
    RotY.M[3][2] = 0;
    RotY.M[3][3] = 1;
    return RotY;
}

FMatrix AJerarquia::MatrizRotacionZ(float angle) {
    FMatrix RotZ;//en este caso la matriz z es la identidad por que theta de V es 0 y eso al realziar calculos es la matriz identdad;
    RotZ.M[0][0] = cos(angle);
    RotZ.M[0][1] = -sin(angle);
    RotZ.M[0][2] = 0;
    RotZ.M[0][3] = 0;
    RotZ.M[1][0] = sin(angle);
    RotZ.M[1][1] = cos(angle);
    RotZ.M[1][2] = 0;
    RotZ.M[1][3] = 0;
    RotZ.M[2][0] = 0;
    RotZ.M[2][1] = 0;
    RotZ.M[2][2] = 1;
    RotZ.M[2][3] = 0;
    RotZ.M[3][0] = 0;
    RotZ.M[3][1] = 0;
    RotZ.M[3][2] = 0;
    RotZ.M[3][3] = 1;
    return RotZ;
}

FMatrix AJerarquia::MultiplicacionMatriz(FMatrix a, FMatrix b) {
    FMatrix c;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c.M[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                c.M[i][j] += a.M[i][k] * b.M[k][j];
            }
        }
    }
    return c;
}

void AJerarquia::ImprimirMatriz(FMatrix m) {
    for (int i = 0; i < 4; i++) {
        UE_LOG(LogClass, Log, TEXT("[%.4f,%.4f,%.4f,%.4f]"), m.M[i][0], m.M[i][1], m.M[i][2], m.M[i][3]);
    }
}

/*
Ejemplo de uso
            if (i & 1) {
                RotacionY = MatrizRotacionY(V->Sons[i]->Phi);
            }
            else {
                RotacionY = MatrizRotacionY(2 * PI - V->Sons[i]->Phi);
            }
            //RotacionY = MatrizRotacionY(2 * PI - V->Sons[i]->Phi);
            //RotacionX = MatrizRotacionX(2 * PI - V->Sons[i]->Phi);
            RotacionZ = MatrizRotacionZ(2 * PI - PI / 2);
            /*if (V->Nivel & 1) {
                RotacionZ = MatrizRotacionZ(PI / 2);
            }
            else {
                RotacionZ = MatrizRotacionZ(2 * PI - PI / 2);
            }* /
            TraslacionV = MatrizTraslacion(V->Sons[i]->X, V->Sons[i]->Y, V->Sons[i]->Z);
            V->Sons[i]->Frame = MultiplicacionMatriz(V->Frame, MultiplicacionMatriz(MultiplicacionMatriz(TraslacionV, RotacionZ), RotacionY));
*/
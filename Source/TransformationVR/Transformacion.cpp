// Fill out your copyright notice in the Description page of Project Settings.

#include "Transformacion.h"

void Transformacion::Trasladar(FVector Traslacion) {
	H = MultiplicacionMatriz(H, MatrizTraslacion(Traslacion.X, Traslacion.Y, Traslacion.Z));
}

void Transformacion::Rotar(FRotator Rotacion) {
	FMatrix RotacionAxis = MatrizRotacionX(Rotacion.Roll);//rotacion en X
	H = MultiplicacionMatriz(H, RotacionAxis);
	RotacionAxis = MatrizRotacionY(Rotacion.Pitch);//rotacion en X
	H = MultiplicacionMatriz(H, RotacionAxis);
	RotacionAxis = MatrizRotacionZ(Rotacion.Yaw);//rotacion en X
	H = MultiplicacionMatriz(H, RotacionAxis);
}

FMatrix Transformacion::HLocal() {
	//calcula y actualiza el H usando las iversa del hwrold del padre
	H = FromWorldToLocal(HW);
	return H;
}

FMatrix Transformacion::HWorld() {
	//calculary aculiza HW  usando el Hworld del padre
	HW = FromLocalToWorld(H);
	return HW;
}

void Transformacion::CalcularHWDesdeH() {
	if (Padre) {//si
		HW = MultiplicacionMatriz(Padre->HW, H);
	}
	else {
		HW = H;
	}
}

void Transformacion::CalcularHDesdeHW() {//las que probablemente use
	if (Padre) {
		H = MultiplicacionMatriz(Padre->HW.Inverse(), HW);
	}
	else {
		H = HW;
	}
}

//la pregunta es: debo almanecera una matriz local y otra matriz referente al origen? o debo almancenar la local y calcular esa respecto al origen siempre?

FMatrix Transformacion::FromWorldToLocal(FMatrix NewHW) {
	//si tengo una matriz local necesito multiplicarla por la inversa de la amtriz del padre, con referencia al aorigen
	if (Padre) {
		return MultiplicacionMatriz(Padre->HWorld().Inverse(), NewHW);
	}
	else {
		return NewHW;
	}
}

FMatrix Transformacion::FromLocalToWorld(FMatrix NewH) {
	//para obtener esto necesito el H respecto al origen del padre, para multiplicarla por mi H
	if (Padre)
		return MultiplicacionMatriz(Padre->HWorld(), NewH);
	else
		return NewH;
}

Transformacion::Transformacion() {
	H = FMatrix::Identity;
	Padre = nullptr;
}

Transformacion::~Transformacion() {
}

FMatrix Transformacion::MatrizTraslacion(float x, float y, float z) {
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

FMatrix Transformacion::MatrizRotacionX(float angle) {
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

FMatrix Transformacion::MatrizRotacionY(float angle) {
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

FMatrix Transformacion::MatrizRotacionZ(float angle) {
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

FMatrix Transformacion::MultiplicacionMatriz(FMatrix a, FMatrix b) {
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

/*void Transformacion::ImprimirMatriz(FMatrix m) {
    for (int i = 0; i < 4; i++) {
        UE_LOG(LogClass, Log, TEXT("[%.4f,%.4f,%.4f,%.4f]"), m.M[i][0], m.M[i][1], m.M[i][2], m.M[i][3]);
    }
}*/

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

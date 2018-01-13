// Fill out your copyright notice in the Description page of Project Settings.

#include "Jerarquia.h"
#include "Transformacion.h"
#include "Engine/Engine.h"
#include "Nodo.h"
#include "Public/UObject/ConstructorHelpers.h"
#include <stack>

// Sets default values
AJerarquia::AJerarquia()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = nullptr;
	TransformacionesPartes.SetNum(10);
	Nodos.SetNum(10);

    static ConstructorHelpers::FClassFinder<ANodo> NodoClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/Nodo_BP.Nodo_BP_C'"));
    if (NodoClass.Succeeded()) {
        if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TipoNodo encontrado."));
        TipoNodo = NodoClass.Class;
    }

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

void AJerarquia::UnirPadreHijo(int IdPadre, int IdHijo) {
	TransformacionesPartes[IdHijo].Padre = &(TransformacionesPartes[IdPadre]);
	TransformacionesPartes[IdPadre].Hijos.Add(&(TransformacionesPartes[IdHijo]));
	//se supone que el hijo es el nuevo nodo a crear, y el padre ya esta creado
	CrearNodo(TransformacionesPartes[IdHijo].ParteAsociada);
	ActualizarNodos();
}

void AJerarquia::Actualizar() {
	//con una busqueda en profundidad, actualizar la jerarquia, multipllicar los frames
	//y establecer nuevas posiciones
	//la pregunta es si estoy haciendo un calculo de puntos de world a local o de local a world, me parece que es la primera opcion
	//asi mismo esta funcion debe actualizar las posciones de los actores en funion de los nuevos calculos
	//POR AHORA calculare la diferencia entre la posicion de la matriz y la posicino del actor, y esa diferencia la aplicare a toda la jerarquia
	//ya que en cada tick solo aplico traslacion al root y luego actualiza, que es esl que se encarga de ajustar al mungo global,
	//por ahora esta funcionalidad esta mal, ya que se deberia aplicar los mismo paso para todos lo selementos incluso la raiz
	std::stack<Transformacion *> pila;
	Root->ActualizarParte();
	for (int i = 0; i < Root->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
		pila.push(Root->Hijos[i]);
	}
	while (!pila.empty()) {
		Transformacion * T = pila.top();
		pila.pop();
		T->Trasladar(TraslacionTemporal);
		T->ActualizarParte();
		for (int i = 0; i < T->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
			pila.push(T->Hijos[i]);
		}
	}
}

void AJerarquia::ActualizarWorlds() {//se supone que alguna matriz la que se se ha visto modificada de forma externa, es decir quiza rote alguna parte, por lo tanto su matriz global cambio
	//en ese mismo instante debo calcular su matriz local, una vez hecho esto debo llamar a esta funcion, que actualizara todo el arbol, aplicando la nueva traslacion de esas matrices donde corresponde, y tambien actualizando las posicones de las partes
	std::stack<Transformacion *> pila;
	pila.push(Root);
	while (!pila.empty()) {
		Transformacion * T = pila.top();
		pila.pop();
		T->CalcularHWorld();
		T->ActualizarParte();
		for (int i = 0; i < T->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
			pila.push(T->Hijos[i]);
		}
	}
	ActualizarNodos();
}

void AJerarquia::CrearNodo(AParte * ParteAsociada) {
        UWorld * const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation(ParteAsociada->Id * 5);
			
			SpawnLocation = GetTransform().TransformPosition(SpawnLocation);
			SpawnLocation += FVector(-300.0f, 0.0f, 200.0f);
			ANodo * const NodoInstanciado = World->SpawnActor<ANodo>(TipoNodo, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			Nodos[ParteAsociada->Id] = NodoInstanciado;
			NodoInstanciado->IdParte = ParteAsociada->Id;//para el texto del numero, quiza este tipo de funcionalidad deberia estar encapsulada en alguna funcion de la clase nodo
			NodoInstanciado->CambiarNombreParte(ParteAsociada->Id);
			//NodoInstanciado->bActualizado = false;
		}

}

void AJerarquia::ImprimirMatriz(FMatrix m) {
    for (int i = 0; i < 4; i++) {
        UE_LOG(LogClass, Log, TEXT("[%.4f,%.4f,%.4f,%.4f]"), m.M[i][0], m.M[i][1], m.M[i][2], m.M[i][3]);
    }
}

void AJerarquia::AplicarLayout() {
	//la calse transformacion son los nodos sobre los que debo calcular estos layouts
	//una vez calculados los puntos, solo debo darselos a la clse nodo
}

void AJerarquia::ActualizarNodos() {
	for (int i = 0; i < Nodos.Num(); i++) {
		if (Nodos[i]) {//si el nodo existe es por que la parte del cuerpo existe
			//Nodos[i]->CambiarTraslacion(TransformacionesPartes[i].GetLocation());
			Nodos[i]->CambiarTraslacion(TransformacionesPartes[i].GetWorldLocation());
		}
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
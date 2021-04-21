/*
 * test__test_group.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */
/**
 * 2021-04-20 20:51
 * Historia Inicializa -- 4 puntos según el último estimado
 * Inicializa un grupo de pruebas. La instancia de grupo de pruebas debe tener referencia a la lista de funciones de prueba pertenecientes al
 * grupo. Además debe tener referencia a funciones de inicialización y finalización de grupo y de prueba.
 * La lista de funciones de prueba mantendrá, además de las funciones, descripciones y estado de cada prueba.
 * El estado podrá ser no ejecutada, exito, falla y error. Falla indica la falla de la prueba y error una falla externa que evitó su ejecución.
 * Las pruebas se implementarán como funciones estáticas en una unidad de compilación dedicada. La misma unidad tendrá una función global para
 * ejecutar la inicialización del grupo de pruebas. El grupo de pruebas no será reentrante, manteniendo su estado en variables static. El estado
 * incluirá al menos un objeto grupo de pruebas y una lista de pruebas que deberá asociarse al objeto durante su inicialización.
 *
 * Debe poder inicializarse un grupo de pruebas incluyendo múltiples pruebas con sus respectivas descripciones. Las pruebas de este grupo serán
 * una primera aproximación a la prueba de capacidades del sistema en desarrollo según el estado del conocimiento actual acerca del mismo.
 *
 * Las pruebas unitarias del sistema serán ejecutadas usando las facilidades en desarrollo, por ello nos apartaremos un tanto de la filosofía TDD
 * en este caso puesto que las pruebas unitarias no podrán ejecutarse hasta que sea implementada la historia correspondiente.
 *
 * 2021-04-20 22:23 Pasa pruebas de aceptación Historia Inicializa
 * 2021-04-21 00:12
 * 	+ STUB para simplificar la escritura de stubs que no hacen nada
 * 	+ Modificación de los nombres para adaptarse a convención
 * 	+ BeforeTest (inicialización solamente)
 * 	+ AfterTest (inicialización solamente)
 * 2021-04-21 00:34
 */

#include <test.h>
#include <string.h> /* memset */
#include <stdio.h> /* printf */

static TestGroup grupo;

#define STUB(nombre) static void nombre(TestGroup *tg)\
{\
	(void) tg;\
}

STUB(inicializadorDeGrupo)
STUB(finalizadorDeGrupo)
STUB(inicializadorDePrueba)
STUB(finalizadorDePrueba)
STUB(pruebaSiempreExitosa)
STUB(pruebaStub)

static TestDescriptor pruebas[]={
		{"Prueba que siempre es exitosa",pruebaSiempreExitosa},
		{"Prueba stub, siempre exitosa", pruebaStub}
};

static int numPruebas = sizeof(pruebas)/sizeof(*pruebas);

enum{VALOR_TESTIGO=0x5a};

static inline void rellenaConValorTestigo(void *puntero, size_t bytes)
{
	memset(puntero,VALOR_TESTIGO,bytes);
}
static inline int byteIgual(const unsigned char *puntero,size_t posicion,int valor)
{
	return puntero[posicion] == valor;
}
static inline int memoriaConservaTestigo(const void *puntero,size_t bytes)
{
	size_t i;

	for(i=0;i<bytes;++i)
		if(!byteIgual(puntero,i,VALOR_TESTIGO))
			return 0;
	return 1;
}

static inline int patronEnPosicion(const unsigned char *memoria,size_t posicion,const void *patron,size_t bytesPatron)
{
	/* memcmp retorna 0 si las dos posiciones de memoria tienen contenidos idénticos */
	return !memcmp(memoria+posicion,patron,bytesPatron);
}
static inline int existePatronEnMemoria(const void *memoria,size_t bytesMemoria,const void *patron,size_t bytesPatron)
{
	size_t i;
	for(i=0;i<=(bytesMemoria-bytesPatron);++i)
		if (patronEnPosicion(memoria,i,patron,bytesPatron))
			return 1;
	return 0;
}

static inline int existePunteroFnEnMemoria(const void *memoria,size_t bytesMemoria,TG_Test test)
{
	TG_Test patron = test;
	return existePatronEnMemoria(memoria,bytesMemoria,&patron,sizeof(patron));
}
static inline int existePunteroDatoEnMemoria(const void *memoria,size_t bytesMemoria,void *pdato)
{
	void *patron = pdato;
	return existePatronEnMemoria(memoria, bytesMemoria, &patron, sizeof(patron));
}

static int falla;
inline static void reportaFalla(char *mensaje)
{
	printf("[FALLA] %s\n",mensaje);
	falla = 1;
}

int testRun_TestGroup(void)
{
	falla=0;
	printf("%d pruebas definidas\n",numPruebas);

	rellenaConValorTestigo(&grupo,sizeof(grupo));

	TG_init(&grupo);
	if(memoriaConservaTestigo(&grupo, sizeof(grupo)))
		reportaFalla("No se ha inicializado la memoria de grupo de prueba!");

	TG_doBeforeGroup(&grupo, inicializadorDeGrupo);
	TG_doAfterGroup(&grupo, finalizadorDeGrupo);

	TG_doBeforeTest(&grupo,inicializadorDePrueba);
	TG_doAfterTest(&grupo,finalizadorDePrueba);

	TG_setTests(&grupo,pruebas,numPruebas);

	if(!existePunteroFnEnMemoria(&grupo,sizeof(grupo),inicializadorDeGrupo))
		reportaFalla("No existe puntero al inicializador de grupo en el estado de grupo de prueba!");
	if (!existePunteroFnEnMemoria(&grupo, sizeof(grupo), finalizadorDeGrupo))
		reportaFalla("No existe puntero al finalizador de grupo en el estado de grupo de prueba!");
	if(!existePunteroFnEnMemoria(&grupo, sizeof(grupo), inicializadorDePrueba))
		reportaFalla("No existe puntero al inicializador de prueba en el estado de grupo de prueba!");
	if(!existePunteroFnEnMemoria(&grupo, sizeof(grupo), finalizadorDePrueba))
		reportaFalla("No existe puntero al finalizador de prueba en el estado de grupo de prueba!");
	if (!existePunteroDatoEnMemoria(&grupo, sizeof(grupo), pruebas))
		reportaFalla("No existe puntaro a lista de pruebas en el estado de grupo de prueba!");
	if (!existePatronEnMemoria(&grupo,sizeof(grupo),&numPruebas,sizeof(numPruebas)))
		reportaFalla("Numero de pruebas no existe en el estado de grupo de prueba!");

	return falla;
}

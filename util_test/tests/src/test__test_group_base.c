/*
 * test__test_group.c
 *
 *  Created on: 20 abr. 2021
 *      Author: Fernando
 */
#define SOURCE_TEST__TEST_GROUP_C
#include <test.h>
#include <string.h> /* memset */
#include <stdio.h> /* printf */
#include <assertions.h> /* TG_fail TG_error */
#include <test_run.h>

static struct EstadoTests_TestGroup{
	TestGroup grupo;
	int falla;
	struct{
		int inicializaciones;
		int pruebasEjecutadas;
		int pruebasCorrectas;
		int pruebasFalladas;
		int errores;
	}contadores;
}estado;


/**
 * Funciones estáticas -- para uso interno
 */

static inline void registraEjecucion(void)
{
	++estado.contadores.pruebasEjecutadas;
}
static inline void registraEjecucionCorrecta(void)
{
	registraEjecucion();
	++estado.contadores.pruebasCorrectas;
}
static inline void registraEjecucionFalla(void)
{
	registraEjecucion();
	++estado.contadores.pruebasFalladas;
}

static inline void registraEjecucionError(void)
{
	registraEjecucion();
	++estado.contadores.errores;
}

static void inicializadorDeGrupo(TestGroup *tg)
{
	(void)tg;
}
static void finalizadorDeGrupo(TestGroup *tg)
{
	(void)tg;
}
static void inicializadorDePrueba(TestGroup *tg)
{
	(void)tg;
}
static void finalizadorDePrueba(TestGroup *tg)
{
	(void)tg;
}

static void pruebaStub(TestGroup *tg)
{
	registraEjecucionCorrecta();
	(void)tg;
}

static void pruebaFalla(TestGroup *tg)
{
	registraEjecucionFalla();
	REPORT_FAIL(tg,"Esta prueba siempre falla!","Es un mock.");
	printf("ESTO NO DEBIERA VERSE\n");
}

static void pruebaConError(TestGroup *tg)
{
	registraEjecucionError();
	REPORT_ERROR(tg, "Un problema externo impidió ejecutar esta prueba","Es un mock.");
	printf("ESTO NO DEBIERA VERSE\n");
}
static TestDescriptor pruebas[]={
		{"Prueba que siempre es exitosa",pruebaStub},
		{"Prueba stub, siempre exitosa", pruebaStub},
		{"Prueba que siempre falla",pruebaFalla},
		{"Esta prueba encuentra algún problema",pruebaConError},
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

inline static void reportaFalla(const char *mensaje)
{
	printf("[FALLA] %s\n",mensaje);
	estado.falla = 1;
}
static inline void inicializaVariablesTestigo(void)
{
	memset(&estado.contadores,0,sizeof(estado.contadores));
}

static inline void verificaConsistenciaInicializacion(void) {
	if (!existePunteroFnEnMemoria(&estado.grupo, sizeof(estado.grupo),
			inicializadorDeGrupo))
		reportaFalla(
				"No existe puntero al inicializador de grupo en el estado de grupo de prueba!");

	if (!existePunteroFnEnMemoria(&estado.grupo, sizeof(estado.grupo), finalizadorDeGrupo))
		reportaFalla(
				"No existe puntero al finalizador de grupo en el estado de grupo de prueba!");

	if (!existePunteroFnEnMemoria(&estado.grupo, sizeof(estado.grupo),
			inicializadorDePrueba))
		reportaFalla(
				"No existe puntero al inicializador de prueba en el estado de grupo de prueba!");

	if (!existePunteroFnEnMemoria(&estado.grupo, sizeof(estado.grupo), finalizadorDePrueba))
		reportaFalla(
				"No existe puntero al finalizador de prueba en el estado de grupo de prueba!");

	if (!existePunteroDatoEnMemoria(&estado.grupo, sizeof(estado.grupo), pruebas))
		reportaFalla(
				"No existe puntero a lista de pruebas en el estado de grupo de prueba!");

	if (!existePatronEnMemoria(&estado.grupo, sizeof(estado.grupo), &numPruebas,
			sizeof(numPruebas)))
		reportaFalla(
				"Numero de pruebas no existe en el estado de grupo de prueba!");
}


static inline void verificaEjecucion(void)
{
	if ((const int)estado.contadores.pruebasEjecutadas != numPruebas)
		reportaFalla("No se han ejecutado todas las pruebas!");
}

static inline void verificaResultados(void)
{
	const TestGroupOutcome *resumen = TG_getTestOutcome(&estado.grupo);

	const int pruebasEjecutadas = TG_countExecuted(&estado.grupo);
	const int pruebasCorrectas = TG_countPassed(&estado.grupo);
	const int pruebasFalladas = TG_countFailed(&estado.grupo);
	const int errores = TG_countErrors(&estado.grupo);

	const int reportaTodoCorrecto = TG_allTestsPassed(&estado.grupo);
	const int esTodoCorrecto =
			estado.contadores.pruebasEjecutadas == estado.contadores.pruebasCorrectas;

	const int resumenCorrecto = resumen->run == pruebasEjecutadas &&
			                    resumen->passed == pruebasCorrectas &&
								resumen->failed == pruebasFalladas &&
								resumen->error == errores;

	if(pruebasEjecutadas != estado.contadores.pruebasEjecutadas)
		reportaFalla("No reporta el número correcto de pruebas ejecutadas.");

	if(pruebasCorrectas != estado.contadores.pruebasCorrectas)
		reportaFalla("No reporta el número correcto de pruebas correctas.");

	if(pruebasFalladas != estado.contadores.pruebasFalladas)
		reportaFalla("No reporta el número correcto de pruebas falladas.");

	if(errores != estado.contadores.errores)
		reportaFalla("No reporta el número correcto de errores.");

	if(reportaTodoCorrecto != esTodoCorrecto)
		reportaFalla("No reporta correctamente si todas las pruebas pasan.");

	if(!resumenCorrecto)
		reportaFalla("No coinciden los reportes individuales con el resumen.");
}

static inline void inicializaPruebas(void)
{
	inicializaVariablesTestigo();

	printf("%d pruebas definidas\n",numPruebas);

	rellenaConValorTestigo(&estado.grupo,sizeof(estado.grupo));

	TG_init(&estado.grupo,"Pruebas ficticias");
	if(memoriaConservaTestigo(&estado.grupo, sizeof(estado.grupo)))
		reportaFalla("No se ha inicializado la memoria de grupo de prueba!");

	TG_setBeforeGroupAction(&estado.grupo, inicializadorDeGrupo);
	TG_setAfterGroupAction(&estado.grupo, finalizadorDeGrupo);

	TG_setBeforeTestAction(&estado.grupo,inicializadorDePrueba);
	TG_setAfterTestAction(&estado.grupo,finalizadorDePrueba);

	TG_setTests(&estado.grupo,pruebas,numPruebas);

	verificaConsistenciaInicializacion();
}

static inline void descartaDosPruebas(void)
{
	if(numPruebas>2) numPruebas-=2;
}


/**
 * API visible desde el exterior
 */

int testRun_TestGroup_base(void)
{
	estado.falla = 0;
	printf("--INICIO--\n");
	printf("Pruebas de la funcionalidad báse del grupo de pruebas.\n\n");
	inicializaPruebas();
	TG_runTests(&estado.grupo);
	verificaEjecucion();
	verificaResultados();

	descartaDosPruebas();

	inicializaPruebas();
	TG_runTests(&estado.grupo);
	verificaEjecucion();
	verificaResultados();

	if (!estado.falla)
		printf("\nPasaron todas las pruebas base.\n");
	else
		printf("\nFallaron las pruebas base.\n");

	printf("--FIN--\n");

	return estado.falla;
}

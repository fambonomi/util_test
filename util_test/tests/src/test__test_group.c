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
 * Inicio 2021-04-21 00:12
 * 	+ STUB para simplificar la escritura de stubs que no hacen nada
 * 	+ Modificación de los nombres para adaptarse a convención
 * 	+ BeforeTest (inicialización solamente)
 * 	+ AfterTest (inicialización solamente)
 * Fin 2021-04-21 00:34
 *
 * Inicio 2021-04-21 09:53
 * Historia Ejecuta Tests -- 3 puntos según el último estimado
 *
 * Partiendo de un grupo de pruebas correctamente inicializado, el sistema deberá permitir la ejecución automatizada de las pruebas del grupo.
 * Registrará el resultado obtenido en cada prueba ejecutada. Al tiempo que ejecuta las pruebas informará las novedades mediante la salida
 * de consola. Para ello mínimamente presentará la descripción de la prueba al iniciarla y un indicador de estado una vez ésta termine.
 * El indicador deberá ser totalmente claro y expresar todos los estados finales descritos en la historia "Inicializa".
 * Al finalizar las pruebas deberá presentar un resumen con el número de pruebas exitosas, falladas y con error.
 * Deberá implementar un mecanismo que permita a la prueba señalar falla y mensaje de error, siendo exitosa en caso de no hacerlo.
 * Inmediatamente después del fallo de una prueba deberá informar por consola la razón declarada del fallo.
 *
 * Pruebas de aceptación
 *
 * Se inicializará y ejecutará un conjunto de pruebas básico que ejercite las capacidades de inicialización y ejecución. Este grupo de pruebas
 * incluirá una o más pruebas exitosas y una o más fallas. Las pruebas deberán ensayar la correcta inicialización del grupo de pruebas y
 * la correcta ejecución de los casos de prueba.
 *
 * -- Pausa -- 2021-04-21 11:13
 * -- Continúa -- 2021-04-21 15:12
 * -- Fin -- 2021-04-21 16:47
 */
#define SOURCE_TEST__TEST_GROUP_C
#include <test.h>
#include <string.h> /* memset */
#include <stdio.h> /* printf */

static struct EstadoTests_TestGroup{
	TestGroup grupo;
	int falla;
	struct{
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
#define STUB(nombre) static void nombre(TestGroup *tg)\
{\
	(void) tg;\
	registraEjecucionCorrecta();\
}

STUB(inicializadorDeGrupo)
STUB(finalizadorDeGrupo)
STUB(inicializadorDePrueba)
STUB(finalizadorDePrueba)
STUB(pruebaSiempreExitosa)
STUB(pruebaStub)


static void pruebaFalla(TestGroup *tg)
{
	registraEjecucionFalla();
	TG_fail(tg,"Esta prueba siempre falla!");
	printf("ESTO NO DEBIERA VERSE\n");
}

static void segundaFalla(TestGroup *tg)
{
	registraEjecucionFalla();
	TG_fail(tg,"Esta es una prueba que debe fallar");
	printf("ESTO NO DEBIERA VERSE\n");
}

static void pruebaConError(TestGroup *tg)
{
	registraEjecucionError();
	TG_error(tg, "Un problema externo impidió ejecutar esta prueba");
	printf("ESTO NO DEBIERA VERSE\n");
}
static TestDescriptor pruebas[]={
		{"Prueba que siempre es exitosa",pruebaSiempreExitosa},
		{"Prueba que siempre falla",pruebaFalla},
		{"Prueba stub, siempre exitosa", pruebaStub},
		{"Otra prueba que falla",segundaFalla},
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

inline static void reportaFalla(char *mensaje)
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

static inline void verificaEjecucionPruebas(void)
{
	if ((const int)estado.contadores.pruebasEjecutadas != numPruebas)
		reportaFalla("No se han ejecutado todas las pruebas!");
}

static inline void verificaResultados(void)
{
	const int pruebasEjecutadas = TG_countExecuted(&estado.grupo);
	const int pruebasCorrectas = TG_countSuccessful(&estado.grupo);
	const int pruebasFalladas = TG_countFailed(&estado.grupo);
	const int errores = TG_countErrors(&estado.grupo);

	if(pruebasEjecutadas != estado.contadores.pruebasEjecutadas)
		reportaFalla("No reporta el número correcto de pruebas ejecutadas.");

	if(pruebasCorrectas != estado.contadores.pruebasCorrectas)
		reportaFalla("No reporta el número correcto de pruebas correctas.");

	if(pruebasFalladas != estado.contadores.pruebasFalladas)
		reportaFalla("No reporta el número correcto de pruebas falladas.");

	if(errores != estado.contadores.errores)
		reportaFalla("No reporta el número correcto de errores.");
}

static inline void inicializaPruebas(void)
{
	inicializaVariablesTestigo();

	printf("%d pruebas definidas\n",numPruebas);

	rellenaConValorTestigo(&estado.grupo,sizeof(estado.grupo));

	TG_init(&estado.grupo,"Pruebas ficticias");
	if(memoriaConservaTestigo(&estado.grupo, sizeof(estado.grupo)))
		reportaFalla("No se ha inicializado la memoria de grupo de prueba!");

	TG_doBeforeGroup(&estado.grupo, inicializadorDeGrupo);
	TG_doAfterGroup(&estado.grupo, finalizadorDeGrupo);

	TG_doBeforeTest(&estado.grupo,inicializadorDePrueba);
	TG_doAfterTest(&estado.grupo,finalizadorDePrueba);

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

int testRun_TestGroup(void)
{
	estado.falla = 0;

	inicializaPruebas();

	TG_runTests(&estado.grupo);

	verificaEjecucionPruebas();
	verificaResultados();

	descartaDosPruebas();

	inicializaPruebas();

	TG_runTests(&estado.grupo);

	verificaEjecucionPruebas();
	verificaResultados();

	return estado.falla;
}

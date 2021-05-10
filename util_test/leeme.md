# Biblioteca simple para desarrollo basado en pruebas, util_test

Esta biblioteca fue desarrollada para su uso en el dictado de la materia Electrónica IV de la carrera de Ingeniería Electrónica en el Departamento de Eléctrica, Electrónica y Computación de la Facultad de Ciencias Exactas y Tecnología de la Universidad Nacional de Tucumán. La nomenclatura usada en la librería está libremente inspirada por la librería JUnit.

El ánimo del desarrollo es contar con una biblioteca de soporte para la confección de pruebas unitarias que sea simple tanto en su uso como en su estructura. Un segundo objetivo fue experimentar la disciplina de desarrollo basado en pruebas (TDD) como un primer paso para su implementación en la currícula.

En notas de desarrollo encontrarán algunos apuntes personales tomados durante el proceso de desarrollo de la librería, incluyendo tiempos empleados en cada actividad. El objetivo de esas notas fue medir los tiempos necesarios para distintas tareas. Si bien se menciona el término scrum, no se empleó el proceso de scrum en modo formal durante el desarrollo, aunque se intentó seguir las distintas actividades de planificación, estimación y retrospectiva en una forma muy simplificada. Esto fue con el objeto de experimentar el proceso y generar ideas para su introducción en el ámbito de una materia que cuenta con únicamente 6 hs de clase semanales.

# Uso

La biblioteca fue escrita íntegramente en lenguaje C y con estilo orientado a objetos. El elemento básico en esta bilbioteca es el grupo de pruebas, TestGroup. Las aserciones fueron desarrolladas en un módulo separado, con una interfaz que define la comunicación entre las aserciones y el TestGroup. La salida fue también implementada en un módulo separado o plugin con una interfaz definida de modo que sea sencillo implementar otros módulos de salida. Si no se configura ningún módulo se empleará uno por defecto que genera salida en STDOUT. La estructura básica para un conjunto de pruebas es la siguiente:

    #include <test.h>
    #include <assertions.h>
    #include <stddef.h> /*size_t*/
    
    /* zona privada */
    static void test_pruebaQuePasa(TestGroup *tg)
    {
        char mem1[] = "Hola";
        char mem2[] = "Hola";
        char mem3[] = "Chau";
        size_t longitud = sizeof(mem1);
        
        ASSERT_TRUE(tg,"Esto debe ser verdadero",1);
        ASSERT_FALSE(tg,"Esto debe ser falso",0);
        ASSERT_INT_EQUAL(tg,"Estos enteros deben ser iguales",10,10);
        ASSERT_INT_NOT_EQUAL(tg,"Estos enteros deben ser distintos",0xAAA,0x555);
        ASSERT_MEM_EQUAL(tg,"Estas regiones de memoria deben tener igual contenido",mem1,mem2,longitud);
        ASSERT_MEM_NOT_EQUAL(tg,"Estas regiones de memoria deben tener distinto contenido");
    }
    static void test_pruebaQueFalla(TestGroup *tg)
    {
        ASSERT_INT_EQUAL(tg,"Estos enteros deben ser iguales",0,1);
    }
    static void test_pruebaQueGeneraError(TestGroup *tg)
    {
        ASSERT_MEM_EQUAL(tg,"Las regiones de memoria deben tener el mismo contenido",NULL,NULL,0);
    }
    
    static TestDescriptor pruebas[]={
        {"Esta prueba debe pasar, es un ejemplo de distintas aserciones",test_pruebaQuePasa},
        {"Esta prueba debe fallar, es un ejemplo de una aserción que falla",test_pruebaQueFalla},
        {"Esta prueba debe indicar error, intenta comparar punteros nulos",test_pruebaQueFalla},
    };
    static const int numPruebas = sizeof(pruebas)/sizeof(*pruebas);
    
    /* zona pública */
    int main(void)
    {
        TestGroup tg;
        TG_init(&tg);
        TG_setTests(&tg,pruebas,numPruebas);
        TG_runTests(&tg);
        
        return !TG_allTestsPassed(&tg);
    }

Se recomienda habilitar todas las advertencias (warnings) en el compilador y activar la opción de generar error en caso de ocurrir una advertencia.

# Dominio público

Este trabajo se libera al dominio público. EL TRABAJO SE HACE DISPONIBLE TAL CUAL, SIN NINGUNA GARANTÍA IMPLÍCITA O EXPLÍCITA.

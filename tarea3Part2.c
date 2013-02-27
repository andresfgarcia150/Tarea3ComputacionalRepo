/**
 * Universidad de los Andes
 * Física computacional
 * Tarea 3 - Parte 2
 * Autor: Andrés Felipe García Albarracín
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_linalg.h>
//#include <gsl/gsl_matrix.h>

void contarDatos(int *nDatos, char nombreArchivo[]);
gsl_vector* cargarVector(int nDatos, int numeroColumna, char nombreArchivo[]);
double calcularMedia(int tamanioVector, gsl_vector *vector);

int main (int argc, char **argv)
{
	// Variables de los vectores
	int nDatos = 0;

	// Revisa que la entrada sea correcta
	if (argc !=2)
	{
		printf("Debe ingresar el nombre del archivo a analizar\n");
		exit(1);
	}
		
	// Variable del archivo
	char nombreArchivo[256];
	strcpy(nombreArchivo,argv[1]);
	
	// Cuenta el número de datos
	contarDatos(&nDatos, nombreArchivo);
	
	// Lee los datos del archivo
	gsl_vector *v1 = gsl_vector_calloc(nDatos);
	v1 = cargarVector(nDatos, 1, nombreArchivo);
	gsl_vector_fprintf (stdout, v1, "%g");

	gsl_vector *v2 = gsl_vector_calloc(nDatos);
	v2 = cargarVector(nDatos, 2, nombreArchivo);
	gsl_vector_fprintf (stdout, v2, "%g");

	gsl_vector *v3 = gsl_vector_calloc(nDatos);
	v3 = cargarVector(nDatos, 3, nombreArchivo);
	gsl_vector_fprintf (stdout, v3, "%g");
	
	// Medias aritméticas
	double u1, u2, u3;
	u1 = calcularMedia(nDatos, v1);
	u2 = calcularMedia(nDatos, v2);
	u3 = calcularMedia(nDatos, v3);
}

// Función que cuenta el número de datos a procesar
void contarDatos(int *nDatos, char nombreArchivo[])
{
	// Variables locales y abre el archivo
	int numDatos = 0;
	FILE *archivo;
	archivo = fopen(nombreArchivo,"r");


	char var = fgetc(archivo);
	// Cuenta el número de datos
	while (var != EOF)
	{
		if (var == '\n')
			numDatos++;
		var = fgetc(archivo);
	}
	
	*nDatos = numDatos;
	fclose(archivo);
}


gsl_vector* cargarVector(int nDatos, int numeroColumna, char nombreArchivo[])
{
	// Variables locales y abre el archivo
	FILE *archivo2;
	archivo2 = fopen(nombreArchivo,"r");

	// Arreglo con los datos de la matriz
	gsl_vector *vector = gsl_vector_calloc(nDatos);	

	// Lee los datos y los guarda en la matriz
	int filaActual = 0;
	for (filaActual = 0; filaActual < nDatos; filaActual++)
	{
		double dato1, dato2, dato3;
		fscanf(archivo2, "%lf %lf %lf\n", &dato1, &dato2, &dato3);
		if (numeroColumna == 1)
			gsl_vector_set(vector, filaActual, dato1);
		else if (numeroColumna == 2)
			gsl_vector_set(vector, filaActual, dato2);
		else if (numeroColumna == 3)
			gsl_vector_set(vector, filaActual, dato3);
	}

	fclose(archivo2);		
	return vector;
}

// Calcula la media aritmética de un vector
double calcularMedia(int tamanioVector, gsl_vector *vector)
{
	double suma = 0;
	int i = 0;
	for (i = 0; i < tamanioVector; i++)
	{
		suma += gsl_vector_get(vector, i);
	}
	return suma/tamanioVector;
}


/**
 * Universidad de los Andes
 * Física computacional
 * Tarea 3
 * Autor: Andrés Felipe García Albarracín
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_linalg.h>
//#include <gsl/gsl_matrix.h>

void contarDatos(int *nDatos, char nombreArchivo[]);
double* cargarVector(int nDatos, int numeroColumna, char nombreArchivo[]);

int main (int argc, char **argv)
{
	// Variables de los vectores
	int nDatos = 0;
		
	// Variable del archivo
	char nombreArchivo[256];
	strcpy(nombreArchivo,argv[1]);
	

	// Cuenta el número de datos
	contarDatos(&nDatos, nombreArchivo);
	
	// Arreglos con los vectores a trabajar
	double* vectorTiempo;
	vectorTiempo = cargarVector(nDatos, 1, nombreArchivo);

	double* vectorPosicion;
	vectorPosicion = cargarVector(nDatos, 2, nombreArchivo);
	
	
	// Vectores para la librería GSL
	gsl_vector *vTiempo = gsl_vector_calloc(nDatos);
	gsl_vector *vPosicion = gsl_vector_calloc(nDatos);
	int i = 0;
	for (i = 0; i < nDatos; i++)
	{
		gsl_vector_set(vTiempo,i,vectorTiempo[i]);
		gsl_vector_set(vPosicion, i, vectorPosicion[i]);
	}

	// Matriz g y su transpuesta gt
	int nColumnas = 3;
	int nFilas = nDatos;
	gsl_matrix *g = gsl_matrix_calloc(nFilas,nColumnas);
	gsl_matrix *gt = gsl_matrix_calloc(nColumnas,nFilas); //En el orden contrario
	for (i = 0; i < nFilas; i++)
	{
		// Matriz g		
		gsl_matrix_set(g, i, 0, 1);
		gsl_matrix_set(g, i, 1, gsl_vector_get(vTiempo,i));
		gsl_matrix_set(g, i, 2, (gsl_vector_get(vTiempo,i)) * (gsl_vector_get(vTiempo,i)) /2 );
		// Matriz gt
		gsl_matrix_set(gt, 0, i, 1);
		gsl_matrix_set(gt, 1, i, gsl_vector_get(vTiempo,i));
		gsl_matrix_set(gt, 2, i, (gsl_vector_get(vTiempo,i)) * (gsl_vector_get(vTiempo,i)) /2 );
		
	}
	
	
	// Matriz x = gt*g, matriz nxn
	gsl_matrix *x = gsl_matrix_calloc(nColumnas,nColumnas);
	int j = 0;
	int h = 0;
	double suma = 0;
	for (i = 0; i < nColumnas; i++) //Filas
	{
		for (j = 0; j < nColumnas; j++) // Columnas
		{
			suma = 0;
			for (h = 0; h < nFilas; h++)
			{
				suma += gsl_matrix_get(gt,i,h)*gsl_matrix_get(g,h,j);
			}
			gsl_matrix_set(x, i, j, suma);
		}
	}
	
	for (i = 0; i<nColumnas; i++)
	{
		printf("%f  %f  %f\n", gsl_matrix_get(x, i, 0), gsl_matrix_get(x,i,1), gsl_matrix_get(x,i,2));
	}
}

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

double* cargarVector(int nDatos, int numeroColumna, char nombreArchivo[])
{
	// Variables locales y abre el archivo
	FILE *archivo2;
	archivo2 = fopen(nombreArchivo,"r");

	// Arreglo con los datos de la matriz
	double *vector;
	vector = malloc((nDatos)*sizeof(double));

	// Lee los datos y los guarda en la matriz
	int filaActual = 0;
	for (filaActual = 0; filaActual < nDatos; filaActual++)
	{
		double dato1;
		double dato2;
		fscanf(archivo2, "%lf %lf\n", &dato1, &dato2);
		if (numeroColumna == 1)
			vector[filaActual] = dato1;
		else if (numeroColumna == 2)
			vector[filaActual] = dato2;
	}

	fclose(archivo2);		
	return vector;
}




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
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
//#include <gsl/gsl_matrix.h>

void contarDatos(int *nDatos, char nombreArchivo[]);
gsl_vector* cargarVector(int nDatos, int numeroColumna, char nombreArchivo[]);
double calcularMedia(int tamanioVector, gsl_vector *vector);
double calcularCovarianza(int tamanioVector, gsl_vector *v1, gsl_vector *v2, double u1, double u2);
double calcularNorma(int tamanioVector, gsl_vector *vector);

int main (int argc, char **argv)
{
	// Variables de los vectores
	int nDatos = 0;
	int nVariables = 3;

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

	// Matriz de covarianza
	double o11, o12, o13, o22, o23, o33;
	o11 = calcularCovarianza(nDatos, v1, v1, u1, u1);
	o12 = calcularCovarianza(nDatos, v1, v2, u1, u2);
	o13 = calcularCovarianza(nDatos, v1, v3, u1, u3);
	o22 = calcularCovarianza(nDatos, v2, v2, u2, u2);
	o23 = calcularCovarianza(nDatos, v2, v3, u2, u3);
	o33 = calcularCovarianza(nDatos, v3, v3, u3, u3);

	gsl_matrix *mCov = gsl_matrix_calloc(nVariables, nVariables);
	gsl_matrix_set(mCov, 0, 0, o11);
	gsl_matrix_set(mCov, 0, 1, o12);
	gsl_matrix_set(mCov, 0, 2, o13);
	gsl_matrix_set(mCov, 1, 0, o12);
	gsl_matrix_set(mCov, 1, 1, o22);
	gsl_matrix_set(mCov, 1, 2, o23);
	gsl_matrix_set(mCov, 2, 0, o13);
	gsl_matrix_set(mCov, 2, 1, o23);
	gsl_matrix_set(mCov, 2, 2, o33);

	// Matriz con vectores propios y vector con valores propios
	gsl_vector *eval = gsl_vector_alloc (nVariables);
        gsl_matrix *evec = gsl_matrix_alloc (nVariables, nVariables);
     
        gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (nVariables);
       
        gsl_eigen_symmv (mCov, eval, evec, w);
     
        gsl_eigen_symmv_free (w);
	
	gsl_eigen_symmv_sort (eval, evec, GSL_EIGEN_SORT_ABS_ASC);

{
int i;
for (i = 0; i < nVariables; i++)
{
double eval_i
= gsl_vector_get (eval, i);
gsl_vector_view evec_i = gsl_matrix_column (evec, i);
printf ("eigenvalue = %g\n", eval_i);
printf ("eigenvector = \n");
gsl_vector_fprintf (stdout,
&evec_i.vector, "%g");
}
}

       	// Ordenamiento de los vectores propios según los valores propios
	// Algoritmo de selección
	int i = 0;
	for (i = 0; i < 2; i++)
	{
		double mayorValor = gsl_vector_get(eval,i);
		int posMayor = i;
		int j = i+1;
		for (j=i+1; j<3; j++)
		{
			if (mayorValor < gsl_vector_get(eval,j))
			{
				posMayor = j;
				mayorValor = gsl_vector_get(eval,j);
			}
		}
		gsl_vector_swap_elements(eval, i, posMayor);
		gsl_matrix_swap_columns(evec, i, posMayor);
	}
	

	{
int i;
for (i = 0; i < nVariables; i++)
{
double eval_i
= gsl_vector_get (eval, i);
gsl_vector_view evec_i = gsl_matrix_column (evec, i);
printf ("eigenvalue = %g\n", eval_i);
printf ("eigenvector = \n");
gsl_vector_fprintf (stdout,
&evec_i.vector, "%g");
}
}
	// Normalización y de los vectores propios
	gsl_vector *vx1 = gsl_vector_calloc (nVariables);
	gsl_vector *vx2 = gsl_vector_calloc (nVariables);
	gsl_vector *vx3 = gsl_vector_calloc (nVariables);

	gsl_matrix_get_col(vx1, evec, 0);
	gsl_matrix_get_col(vx2, evec, 1);
	gsl_matrix_get_col(vx3, evec, 2);
	
	double norm1, norm2, norm3;
	norm1 = calcularNorma(nVariables, vx1);
	norm2 = calcularNorma(nVariables, vx2);
	norm3 = calcularNorma(nVariables, vx3);
	
	gsl_vector_scale(vx1,1/norm1);
	gsl_vector_scale(vx2,1/norm2);
	gsl_vector_scale(vx3,1/norm3);

	
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

// Calcular covarianza
double calcularCovarianza(int tamanioVector, gsl_vector *v1, gsl_vector *v2, double u1, double u2)
{
	double suma = 0;
	int i = 0;
	for (i = 0; i < tamanioVector; i++)
	{
		suma += (gsl_vector_get(v1,i)-u1) * (gsl_vector_get(v2,i)-u2);
	}
	return suma/(tamanioVector-1);
}

// Calcula la norma de un vector
double calcularNorma(int tamanioVector, gsl_vector *vector)
{
	double suma = 0;
	int i = 0;
	for (i = 0; i < tamanioVector; i++)
	{
		suma += pow((gsl_vector_get(vector,i)),2);
	}
	return pow(suma,0.5);
}


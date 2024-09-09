#include "ruido.h"
#include "utils.h"

/* Exercício proposto pelo professor Rodrigo Minetto - UTFPR */
Image* create (int nrows, int ncols) {
  int i;
  Image *img = (Image *)malloc(sizeof(Image)); 
  img->nrows = nrows;
  img->ncols = ncols;
  img->m = (int **)malloc(ncols * sizeof(int *));
  for (i = 0; i < ncols; i++) {
    img->m[i] = (int *)malloc(nrows * sizeof(int)); 
  }
  return img;
}

/* */
void destroy (Image *img) {
  int i;
  for (i = 0; i < img->ncols; i++) {
    free(img->m[i]);
  }
  free(img->m);
  free(img);
}

/* */
Image* read_image (FILE *file_in, int *nrows, int *ncols) {
  char *type = (char *)malloc(sizeof(char));
  fscanf(file_in, "%s", type);   
  fscanf(file_in, "%d %d", ncols, nrows);   
  Image *in = create (*nrows, *ncols);
  int i, j;
  for (j = 0; j < in->nrows; j++) {
    for (i = 0; i < in->ncols; i++) {
      fscanf(file_in, "%d", &(in->m[i][j]));
    }
  } 
  free(type);
  return in;
}

/* */
void write_image (FILE *file_out, Image *out, int nrows, int ncols) {
  int i, j;	
  fprintf(file_out, "P2\n%d %d\n255\n", out->ncols, out->nrows);
  for (j = 0; j < out->nrows; j++) {
    for (i = 0; i < out->ncols; i++) {
      fprintf(file_out, "%d ", out->m[i][j]);
      if ( (i != 0) && ((i % 12) == 0) ) {
        fprintf(file_out, "\n");
      }
    }
  }
}

/* */  
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/* */
Image* process (Image *in, int wsize) {
  int i, j, k = 0, l, p;
  Image *out = create (in->nrows, in->ncols);
  /*Array {A} é usado para armazenar os pixels da janela*/
  /*O array {A} deve ser ordenado e utilizado para mediana*/
  int *A = (int *)malloc(wsize * wsize * sizeof(int));
  for (i = wsize; i < (in->ncols - wsize); i++) {
    for (j = wsize; j < (in->nrows - wsize); j++) {
      for (l = i - wsize/2; l <= i + wsize/2; l++) {
        for (p = j - wsize/2; p <= j + wsize/2; p++) {
          A[k++] = in->m[l][p];
        }  
      }
      insertionSort (A, wsize * wsize);
      out->m[i][j] = A[wsize * wsize/2];
      k = 0;
    }
  }
  free(A); 
  return out;
}

/* */
int main ( ) {
  int nrows, ncols;
  FILE *file_in  = fopen("lena.pgm", "r");
  FILE *file_out = fopen("saida.pgm", "w");
  /*tamanho da janela de remoção de ruído: */
  int wsize = 3; 
  Image *in = read_image (file_in, &nrows, &ncols);
  Image *out = process (in, wsize);
  write_image (file_out, out, nrows, ncols);
  destroy (in); 
  destroy (out);
  fclose(file_in);
  fclose(file_out);
  return 0;
}



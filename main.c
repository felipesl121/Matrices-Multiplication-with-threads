#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>


int **matrixA=NULL;
int **matrixB=NULL;
int **matrixC=NULL;

int TAM;
int NumThr;

void *funcao(void *arg)
{
  int i,j,x;
  int id=*((int *)arg);
  int qtd=TAM/NumThr;
  int fim=qtd*(id+1);
  //printf("Olá mundo! %d\n",id);

  for(i=id*qtd;i<fim;i++)
  {
    for(j=0;j<TAM;j++)
    {
        matrixC[i][j] = 0;
        for(x=0; x<TAM; x++)
        {
        matrixC[i][j] = matrixC[i][j] + (matrixA[i][x] * matrixB[x][j]);
      }   
    }
  }

   return (NULL);
}
 
void preenche()
{
  int i,j;
  srand(time(NULL));
  for (i=0;i<TAM;i++)
  {
    for (j=0;j<TAM;j++)
    {
      matrixA[i][j]=rand()%10;
    }
  }

  for (i=0;i<TAM;i++)
  {
    for (j=0;j<TAM;j++)
    {
      matrixB[i][j]=rand()%10;
    }
  }
}
int main(int argc, char const *argv[])
{
  int i;
  struct timeval ini,fim;
  float TIME;


  pthread_t *thread;
  pthread_attr_t *attr;

  if (argc>2)
  {
    TAM=atoi(argv[1]);
    NumThr=atoi(argv[2]);
    printf("NumThr: %d\n", NumThr);
        
  }else
  {
    printf("Falta argumento\n");
    printf("USE: ./multiplic_matrix [num ordem matrix] [num thread]\n"); 
    exit(-1);
  }
  if(NumThr > TAM || NumThr < 1)
  {
    printf("Erro no num de threads\n");
    exit(-2);
  }
  int a[NumThr];

  attr=malloc(sizeof(pthread_attr_t)*NumThr);
  thread=malloc(sizeof(pthread_t)*NumThr);

  matrixA=malloc(TAM*sizeof(int*)); /*Aloca a matrix A*/
  matrixB=malloc(TAM*sizeof(int*));  /*Aloca a matrix B*/
  matrixC=malloc(TAM*sizeof(int*)); /*Aloca a matrix C*/
  
  for(i=0;i<TAM;i++)
  {
    matrixA[i]=malloc(TAM*sizeof(int));
    matrixB[i]=malloc(TAM*sizeof(int));
    matrixC[i]=malloc(TAM*sizeof(int));
  }

  preenche();
  
  for(i=0;i<NumThr;i++)
  { 
    pthread_attr_init(&attr[i]);
  }

  for(i=0;i<NumThr;i++)
  {
    a[i]=i;
  }
  gettimeofday(&ini,NULL);
    for(i=0;i<NumThr;i++)
    {
      pthread_create(&thread[i],&attr[i],funcao,&a[i]);
    }
    
    for(i=0;i<NumThr;i++)
    {
      pthread_join(thread[i],NULL);  /* Esperara a junção das threads */ 
    }
    gettimeofday(&fim,NULL);
  TIME=(float)(fim.tv_sec-ini.tv_sec);
  TIME=TIME+(float)(fim.tv_usec-ini.tv_usec)/1000000.0;
  printf("Tempo: %.6f\n", TIME);
  
  for(i=0;i<TAM;i++)
  {
  	free(matrixA[i]);
  	free(matrixB[i]);
  	free(matrixC[i]);
  }
  free(matrixA);
  free(matrixB);
  free(matrixC);


  return 0;   /* O programa chegará aqui. */ 
}

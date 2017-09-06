//MPI code
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[])
{
  int nop,s=240,master=0,t1,t2,offset,col,source,mtype,numtasks,rank=0;
  fprintf(stderr,"Main Started\n");

  MPI_Status status;
  fprintf(stderr,"Main Init\n");
  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

  nop=numtasks-1;
  int a[s][s],b[s][s],c[s][s];
  double d1, d2, d3, d4, d5, d6;
  //intialise arrays                                                                                                                                                                           
  if(rank==master)
    {  d1= MPI_Wtime();
      fprintf(stderr,"%d : Array Init\n",rank);
      for(int i=0;i<s;i++)
        {
          for(int j=0;j<s;j++)
            {
              a[i][j]=rand()%5;
              b[i][j]=rand()%5;
            }
        }

      t1=s/nop;
      t2=s%nop;
                                                                                                                                    
      offset = 0;
        
      for(int dest=1; dest<=nop; dest++)
        {
          fprintf(stderr,"%d \n",rank,dest);
          col= (dest<= t2) ? t1+1 : t1;
            
          MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
          MPI_Send(&col, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
          MPI_Send(&a, s*s, MPI_INT, dest, mtype, MPI_COMM_WORLD);
          MPI_Send(&b[0][offset],s*col, MPI_INT, dest, mtype,MPI_COMM_WORLD);
          offset = offset + col;
        }
        
      for(int i=1; i<=nop; i++)
        {
          if(i==2)
             d3=MPI_Wtime();
          source= i;
          fprintf(stderr,"%d \n",rank,i);
          MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
          MPI_Recv(&col, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
          MPI_Recv(&c[0][col], s*col, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            
          if(i==2)
            d4=MPI_Wtime();
        }
        
        
      for(int i=0; i<s; i++)
        {
          printf("\n");
          for(int j=0; j<s; j++)
            printf("%d", c[i][j]);
        }
        
        
      d2=MPI_Wtime();
    }

  if(rank>MASTER)
    {   
      if(rank==2)
        d5=MPI_Wtime();
        
      fprintf(stderr,"%d \n",rank);
      MPI_Recv(&offset, 1, MPI_INT, master, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&col, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&c[0][col], s*col, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
        
      if(i==2)
        d4=MPI_Wtime();
    }
    
   for(int i=0; i<s; i++)
    {
      printf("\n");
      for(int j=0; j<s; j++)
        printf("%d", c[i][j]);
    }
    
    
   d2=MPI_Wtime();
    

  if(rank>master)
    {   if(rank==2)
        d5=MPI_Wtime();
      // From master                                                                                                                                                                    
      fprintf(stderr,"%d \n",rank);
      MPI_Recv(&offset, 1, MPI_INT, master, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&col, 1, MPI_INT, master, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, s*s, MPI_INT, master, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, s*col, MPI_INT, master, mtype, MPI_COMM_WORLD, &status);
      fprintf(stderr,"%d \n",rank);
      for(int x=0; x<s; x++)
      {
        for(int i=0; i<col; i++)
          {
            for(int j=0; j<s; j++)
              c[i][x] = c[i][x] + a[i][j] * b[j][x];
          }
      }
      // From worker                                                                                                                                                                   
      fprintf(stderr,"%d : sending-----\n",rank);
      MPI_Send(&offset, 1, MPI_INT, master, mtype, MPI_COMM_WORLD);
      MPI_Send(&col, 1, MPI_INT, master, mtype, MPI_COMM_WORLD);
      MPI_Send(&c,s*col, MPI_INT, master, mtype, MPI_COMM_WORLD);
      fprintf(stderr,"%d : Sent-----\n",rank);
      if(rank==2)
        d6=MPI_Wtime();
    }
  fprintf(stderr,"Printing time\n");
  fprintf(stderr,"%f \n",d2-d1);

  fprintf(stderr,"%f \n",d6-d5);
  MPI_Finalize();
  return 0;
}
                                          

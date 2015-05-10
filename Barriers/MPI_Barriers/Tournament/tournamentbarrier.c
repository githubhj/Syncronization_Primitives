#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <sys/time.h>
#include "tournamentbarrier.h"

int players;
int rounds;

enum roles {WINNER, LOSER, CHAMPION, WALKOVER, DROPOUT};

typedef struct player_struct
{
	unsigned int opponent;
	enum roles role;
}player;

player tournament[100][10];

void tournamentbarrier_init()
{
	MPI_Comm_size(MPI_COMM_WORLD, &players); //initialize number of nodes

	rounds = ceil( log(players)/log(2) );

	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for(int i=0; i<=rounds; i++)
	{
		int r = pow(2,i);
		int k = pow(2,i-1);

		if((i > 0) && (rank%r==0) && ((rank + k)< players) && (r < players)){
			tournament[rank][i].role = WINNER;
			tournament[rank][i].opponent = rank + k;
		}

		if((i > 0) && (rank%r == 0) && ((rank + k)) >= players){
			tournament[rank][i].role = WALKOVER;
			tournament[rank][i].opponent = -1;
		}

		if((i > 0) && ((rank%r == k))){
			tournament[rank][i].role = LOSER;
			tournament[rank][i].opponent = rank - k;
		}

		if((i > 0) && (rank==0) && (r >= players)){
			tournament[rank][i].role = CHAMPION;
			tournament[rank][i].opponent = rank + k;
		}

		if( i==0 ) {
			tournament[rank][i].role = DROPOUT;
			tournament[rank][i].opponent = -1;
		}

 	}

}

void tournamentbarrier()
{

	if(players == 1) return;

	int i = 1;
	int msg = 1; //arbit message
	MPI_Status *status;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//ARRIVAL
	while(1)
	{
		if(tournament[rank][i].role == WINNER)
		{
			MPI_Recv( &msg, 1, MPI_INT, tournament[rank][i].opponent, 1, MPI_COMM_WORLD, NULL );
		}
		else if(tournament[rank][i].role == LOSER)
		{
			MPI_Send(&msg,1,MPI_INT,tournament[rank][i].opponent,1,MPI_COMM_WORLD);
			MPI_Recv(&msg,1,MPI_INT,tournament[rank][i].opponent,1,MPI_COMM_WORLD,NULL);
			break;
		}
		else if(tournament[rank][i].role == CHAMPION)
		{
			MPI_Recv(&msg,1,MPI_INT,tournament[rank][i].opponent,1,MPI_COMM_WORLD,NULL);
			MPI_Send(&msg,1,MPI_INT,tournament[rank][i].opponent,1,MPI_COMM_WORLD);
			break;
		}	

		if(i <= rounds) i++;
	}

	//WAKEUP
	while(1) {
		if( i > 0 ) i--;

		if( tournament[rank][i].role == WINNER ) {
			MPI_Send(&msg,1,MPI_INT,tournament[rank][i].opponent,1,MPI_COMM_WORLD);
		}

		if( tournament[rank][i].role == DROPOUT ){
			break;
		}
	}		

}

	
	

	




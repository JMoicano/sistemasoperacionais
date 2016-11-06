#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

	if ( argc != 3 ){
		printf("\nusageT\n");
		return 0;
	}
	

	int valor1=0,valor2=0;
	int i = 0;

	valor1 = atoi(argv[1]);
	valor2 = atoi(argv[2]);

	for(i=0 ; i < valor2 ; i++){
		sleep(valor1);
	}

	return 0;

}

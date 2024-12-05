#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480

#define FRAME_BUFFER_DEVICE	"/dev/fb0"

int16_t frame[FRAME_HEIGHT][FRAME_WIDTH];
int id1;
int id2;
int id3;
int idsum;
char name1 [50]  = "Lee Hung Yun\n";
char name2[50]  = "Lin Hui Yu\n";
char name3 [50]  = "Chen Chia Yin\n";
int main()
{
	//RGB16


	int max_cX = -700;
	int min_cY = 270;

	int cY_step = -5;
	int cX = -700;	// x = -700~-700
	int cY;			// y = 400~270

	int fd;


	printf( "Function1: Name\n" );

	name(name1,name2,name3);

	printf( "Function2: ID\n" );

	id(id1,id2,id3,idsum);

	printf( "Main Function:\n" );
	printf( "*****Print All*****\n" );
	printf( "Team 05\n" );
	printf( "%d %s",id1,name1 );
	printf( "%d %s",id2,name2 );
	printf( "%d %s",id3,name3);
	printf( "ID Summation = %d\n",idsum );
	printf( "*****End Print*****\n" );


	printf( "\n***** Please enter p to draw Julia Set animation *****\n" );
	while(getchar()!='p') {}

	system( "clear" );

	fd = open( FRAME_BUFFER_DEVICE, (O_RDWR | O_SYNC) );

	if( fd<0 )
	{	printf( "Frame Buffer Device Open Error!!\n" );	}
	else
	{
		for( cY=400 ; cY>=min_cY; cY = cY + cY_step ) {

			drawJuliaSet( cX, cY, FRAME_WIDTH, FRAME_HEIGHT, frame );

			write( fd, frame, sizeof(int16_t)*FRAME_HEIGHT*FRAME_WIDTH );

			lseek( fd, 0, SEEK_SET );
		}



		printf( ".*.*.*.<:: Happy New Year ::>.*.*.*.\n" );
		printf( "by Team 05\n" );
		printf( "%d %s",id1,name1 );
        printf( "%d %s",id2,name2 );
        printf( "%d %s",id3,name3);

		close( fd );
	}

	while(getchar()!='p') {}

	return 0;
}

#include "SDL2/SDL.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h>
#include <limits.h>
#include <time.h>
#include <conio.h> 
#include <string.h>
#include <stdbool.h>
#include "uint.h"
#include "random_functions.h"


#define MAXX 639.0
#define MAXY 479.0

#define UNUSED(expr) do { (void)(expr); } while (0)




uint32 *createdata(int num,char mode)
{
    int i = 0;
    uint32 *ptr;
	static random_function random_functions[]={system_rand,rand_lsfr,randomMT,autoranval,lcrand,rand_with_array,xorshift,rand_qpr,conc_16_random};

    ptr = (uint32 *)malloc(sizeof(uint32)*num);
	random_function selected_random_function = random_functions[mode-1];

    if(ptr != NULL)
    {
        for(i = 0; i < num; i++)
        {
            ptr[i] = selected_random_function()%100000;
        }
    }
    return ptr;
}

typedef struct{
	double xshift;
	double yshift;
	double zoom;
	double phata;
} CameraData;

bool handle_events(CameraData *);
void update_data(uint8 * data,int width, int height, uint32 * itable,int data_size, CameraData * camData);

int get_int_input(){
	int input=0;
	bool valid_input = false;
	while(!valid_input){
		int result = scanf("%d", &input);
		valid_input=true;
		if (result == EOF) {
			valid_input = false;
			/* ... you're not going to get any input ... */
		}
		if (result == 0) {
			valid_input = false;
			while (fgetc(stdin) != '\n') // Read until a newline is found
				;
		}
	}
	return input;
}

int main(int argc, char * args[]) {
	UNUSED(argc);
	UNUSED(args);
	init_bob_rand();
    seedMT(4357U);
	srand(time(NULL));
	s_lcrand(time(NULL));
	s_rand_qpr(0, 0);
	int selected_mode = 999999;
	while(selected_mode>9){
		printf("Select random function by pressing number key\n1:Windows random \t2:LFSR \t3:Mersenne twister \t4BobByrtle \t5Linear Congruential\t6Random with array \t7xorShift \t8Quadratic Resides \t9Concatenate 16\n");
		selected_mode = get_int_input();
	}
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * sdlWindow = SDL_CreateWindow("My Game Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        680, 520,
        SDL_WINDOW_OPENGL);


    if (sdlWindow == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer * sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

    if (sdlRenderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);

    SDL_Texture * sdlTexture = SDL_CreateTexture(sdlRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        640, 480);
    if (sdlTexture == NULL) {
        printf("Could not create texture: %s\n", SDL_GetError());
        return 1;
    }
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
	
	
	uint8 * data = (uint8 * ) malloc(sizeof(uint8) * 640 * 480);
    uint32 * pixels = (uint32 * ) malloc(sizeof(uint32) * 640 * 480);
    memset(pixels, 0, 640 * 480 * sizeof(uint32));
	memset(data, 0, 640 * 480 * sizeof(uint8));
	int cnt=5999999;
	uint32 * itable =createdata(cnt,selected_mode);
	
	CameraData camData;
	camData.xshift=0;
	camData.yshift=0;
	camData.zoom=1;
	camData.phata=0;
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	
	
	SDL_Rect dstrect;
	dstrect.x = 10;
	dstrect.y = 10;
	dstrect.w = 640;
	dstrect.h = 480;
	int arr_size = 640 * 480;
	bool keep_running = true;
    while (keep_running) {
		memset(pixels, 0, 640 * 480 * sizeof(uint32));
		update_data(data,640,480,itable,cnt,&camData);
		for(int i=0;i<arr_size;++i){
			uint8 data_value = data[i];
			if(data_value){
				pixels[i] = SDL_MapRGBA( format, 0xFF, 0xFF, 0xFF, data_value );

			}
		}
		SDL_UpdateTexture(sdlTexture, NULL, pixels, 640 * sizeof(Uint32));
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &dstrect);
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(20);
		keep_running = handle_events(&camData);
	}
    SDL_DestroyWindow(sdlWindow);

    SDL_Quit();

    free(pixels);
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    return 0;
}

void update_data(uint8 * data,int width, int height, uint32 * itable,int data_size, CameraData * camData){
	double nth,n1th = 0,n2th = 0,n3th = 0,x,y,x1,y1,z1;
	double max_x = width-1.0;
	double max_y = height-1.0;
	memset(data, 0, width * height * sizeof(uint8));
	double usex=sin(camData->phata);
	double usey=cos(camData->phata);
	double x_mult = max_x * camData->zoom / 65535.0 ;
	double y_mult = max_y * camData->zoom / 65535.0;
	double z_mult = max_y * camData->zoom / 65535.0;
	double half_width = max_x/2;
	double half_height = max_y/2;
	for(int num=0;num<data_size;++num){
		nth=itable[num];
		if(num>4){
			x1=(nth - n1th)  * x_mult ;
			y1=(n1th - n2th) * y_mult ;
			z1=(n2th - n3th) * z_mult ;
			x=(x1*usey+z1*usex);
			y=y1+(z1*usey-x1*usex)/2;
			x+=camData->xshift+half_width;
			y+=camData->yshift+half_height;
			if(x>=0&&x<=max_x && y>=0 && y<=max_y){
				int element_index = (width * ((int)y) + (int)x);
				uint8 old = data[element_index];
				uint8 new_value;
					if(old>245){
						new_value = 255;
					}
					else{
						new_value = old+2;
					}
					data[element_index]=new_value;
			}
		}
		n3th=n2th; n2th=n1th;n1th=nth;
	}
}

bool handle_events(CameraData * camData){
	SDL_Event event;
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while( SDL_PollEvent( &event ) ){
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type ){
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
				case SDLK_LEFT:
					camData->xshift+=(MAXY/10)/camData->zoom; break;
					break;
				case SDLK_RIGHT:
					camData->xshift-=(MAXY/10)/camData->zoom; break;
					break;
				case SDLK_UP:
					camData->yshift+=(MAXY/10)/camData->zoom; break;
					break;
				case SDLK_DOWN:
					camData->yshift-=(MAXY/10)/camData->zoom; break;
					break;
				case SDLK_PLUS:
					camData->zoom*=1.5; camData->yshift*=1.5; camData->xshift*=1.5;break;
					break;
				case SDLK_MINUS:
					camData->zoom/=1.5; camData->yshift/=1.5; camData->xshift/=1.5; break;
					break;
				case SDLK_q:
					camData->phata+=0.1; break;
					break;
				case SDLK_e:
					camData->phata-=0.1; break;
					break;
				case SDLK_ESCAPE:
					return false;
				default:
					break;
			}
				break;

			case SDL_KEYUP:
				break;
				
			default:
				break;
		}
	}
	return true;
}
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <stdio.h>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int tpos[200];

int utama(SDL_Event *evt);
SDL_Renderer* grenderer = NULL;
Mix_Music *musik=NULL;
Mix_Chunk *suara=NULL;

struct status{
    int xmakanan;
    int ymakanan;
    int cx;
    int cy;
    int wtulisan;
    int htulisan;
    int tombol;
    int potongan;
    int pkepala;
    int main;
    int selesai;
    SDL_Texture* gambar = NULL;
    SDL_Texture* kalah = NULL;
}stat;

int ganti_tombol(int x){
    stat.tombol=x;
    return 0;
}

int acakmakanan(){
int tmp;
int i;
    srand(SDL_GetTicks()+time(0));
    tmp=rand()%(16*12-stat.potongan);
    for(i=0;i<200;i++){
        if(tmp <= 0){
            break;
        }
        if(tpos[i]==0)tmp--;
    }
    stat.xmakanan=i%16;
    stat.ymakanan=i/16;
    return 0;
}

int gerak() {
int i=0, x, y, dapat_makan=0;
    switch(stat.tombol){
        case SDLK_w:
            if((stat.pkepala/16)>0){
                if(tpos[stat.pkepala-16]==0){
                    if((stat.pkepala-16)==(stat.ymakanan*16+stat.xmakanan)){
                        stat.pkepala-=16;
                        stat.potongan++;
                        tpos[stat.pkepala]=stat.potongan;

                        dapat_makan=1;
                    }else{
                        for(y=0;y<12;y++){
                            for(x=0;x<16;x++){
                                if(tpos[i]>0)tpos[i]--;
                                i++;
                            }
                        }
                    }
                    stat.pkepala-=16;
                    tpos[stat.pkepala]=stat.potongan;
                }else if(tpos[stat.pkepala-16]!=stat.potongan-1)stat.main=0;//kalah
            }else{
                //kalah
                stat.main=0;
                stat.selesai=1;
            }
            break;
        case SDLK_a:
            if((stat.pkepala%16)>0){
                if(tpos[stat.pkepala-1]==0){
                    if((stat.pkepala-1)==(stat.ymakanan*16+stat.xmakanan)){
                        stat.pkepala--;
                        stat.potongan++;
                        tpos[stat.pkepala]=stat.potongan;
                        dapat_makan=1;
                    }else{
                        for(y=0;y<12;y++){
                            for(x=0;x<16;x++){
                                if(tpos[i]>0)tpos[i]--;
                                i++;
                            }
                        }
                        stat.pkepala--;
                        tpos[stat.pkepala]=stat.potongan;
                    }
                }else if(tpos[stat.pkepala-1]!=stat.potongan-1)stat.main=0;//kalah
            }else{
                //kalah
                stat.main=0;
                stat.selesai=1;
            }
            break;
        case SDLK_d:
            if((stat.pkepala%16)<15){
                if(tpos[stat.pkepala+1]==0){
                    if((stat.pkepala+1)==(stat.ymakanan*16+stat.xmakanan)){
                        stat.pkepala++;
                        stat.potongan++;
                        tpos[stat.pkepala]=stat.potongan;
                        dapat_makan=1;
                    }else{
                        for(y=0;y<12;y++){
                            for(x=0;x<16;x++){
                                if(tpos[i]>0) tpos[i]--;
                                i++;
                            }
                        }
                        stat.pkepala++;
                        tpos[stat.pkepala]=stat.potongan;
                    }
                }else if(tpos[stat.pkepala+1]!=stat.potongan-1)stat.main=0;//kalah
            }else{
                //kalah
                stat.main=0;
                stat.selesai=1;
            }
            break;
        case SDLK_s:
            if((stat.pkepala/16)<11){
                if(tpos[stat.pkepala+16]==0){
                    if((stat.pkepala+16)==(stat.ymakanan*16+stat.xmakanan)){
                        stat.pkepala+=16;
                        stat.potongan++;
                        tpos[stat.pkepala]=stat.potongan;
                        dapat_makan=1;
                    }else{
                        for(y=0;y<12;y++){
                            for(x=0;x<16;x++){
                                if(tpos[i]>0)tpos[i]--;
                                i++;
                            }
                        }
                        stat.pkepala+=16;
                        tpos[stat.pkepala]=stat.potongan;
                    }
                }else if(tpos[stat.pkepala+16]!=stat.potongan-1)stat.main=0;//kalah
            }else{
                //kalah
                stat.main=0;
                stat.selesai=1;
            }
            break;
        default:
            break;
    }
    if(dapat_makan!=0){
        Mix_Pause(1);
        Mix_PlayChannel(1, suara, 0);
        acakmakanan();
    }
    return 0;
}

int mulai_game(){
int i=0;
    for(i=0;i<200;i++){
        tpos[i]=0;
    }

    tpos[34]=1;
    tpos[35]=2;
    tpos[36]=3;

    stat.pkepala=36;
    stat.potongan=3;
    acakmakanan();
    stat.selesai=0;
    stat.main=1;
    return 0;
}

int gambar_cacing(SDL_Renderer *latar, SDL_Texture *gambar){
int i=0;
int x, y;
SDL_Rect rct, rcd;
    for(y=0;y<12;y++){
        for(x=0;x<16;x++){
            if(i!=stat.pkepala){
                rct.x=0;
            }else{
                rct.x=stat.cx;
            }
            if(tpos[i]!=0){
                rcd.x=x*stat.cx;
                rcd.y=y*stat.cy;
                rcd.w=stat.cx;
                rcd.h=stat.cy;
                rct.y=0;
                rct.w=stat.cx;
                rct.h=stat.cy;

                SDL_RenderCopy(grenderer, gambar, &rct, &rcd);
            }
            i++;
        }
    }
    return 0;
}

int gambar_makanan(SDL_Renderer *latar, SDL_Texture *gambar){
SDL_Rect rct, rcd;
    rcd.x=stat.xmakanan*stat.cx;
    rcd.y=stat.ymakanan*stat.cy;
    rcd.w=stat.cx;
    rcd.h=stat.cy;

    rct.x=3*stat.cx;
    rct.y=0;
    rct.w=stat.cx;
    rct.h=stat.cy;

    SDL_RenderCopy(grenderer, gambar, &rct, &rcd);
    return 0;
}

int main( int argc, char* args[] ){
	SDL_Window *window = NULL;
	SDL_Surface *gambar1=NULL;
	SDL_Surface *gambar2=NULL;
	TTF_Font *huruf;
	SDL_Rect rcd;
	SDL_Color warna;
	Uint32 wberikutnya;

    warna.r=0x00;
    warna.g=0x00;
    warna.b=0xFF;

	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) <0){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}else{
        mulai_game();
        acakmakanan();

		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( window != NULL ){
            SDL_Event sevent;

			//Get window surface
			TTF_Init();
			Mix_Init(MIX_INIT_MP3);
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
			grenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor( grenderer, 0xFF, 0x00, 0xFF, 0xFF );
            SDL_RenderClear(grenderer);

            huruf=TTF_OpenFont("FreeSans.ttf", 48);
            suara=Mix_LoadWAV("efek.wav");
            musik=Mix_LoadMUS("musik.mp3");
            gambar1=IMG_Load("box.jpg");

            Mix_VolumeMusic(5);
            Mix_VolumeChunk(suara, 5);
            Mix_PlayMusic(musik, -1);

            if(huruf!=NULL){
                gambar2=TTF_RenderText_Blended(huruf, "Sampeyan kalah", warna);
                stat.wtulisan=gambar2->w;
                stat.htulisan=gambar2->h;
            }

            if(musik!=NULL && gambar1!=NULL && gambar2!=NULL && suara!=NULL){
                stat.gambar=SDL_CreateTextureFromSurface(grenderer, gambar1);
                stat.cx=gambar1->w/9;
                stat.cy=gambar1->h;

                SDL_FreeSurface(gambar1);

                stat.kalah=SDL_CreateTextureFromSurface(grenderer, gambar2);
                SDL_FreeSurface(gambar2);
                stat.tombol=SDLK_d;
                gambar_cacing(grenderer, stat.gambar);
                wberikutnya=SDL_GetTicks()+300;
                while(sevent.type!=SDL_QUIT){
                    while(SDL_PollEvent(&sevent)){
                        utama(&sevent);
                    }

                    //Clear screen
                    SDL_SetRenderDrawColor( grenderer, 0xFF, 0xFF, 0x00, 0xFF );
                    SDL_RenderClear(grenderer);

                    rcd.x=0;
                    rcd.y=0;
                    rcd.w=16*stat.cx+4;
                    rcd.h=12*stat.cy+4;
                    SDL_SetRenderDrawColor( grenderer, 0xFF, 0x00, 0x00, 0x00);
                    SDL_RenderFillRect(grenderer, &rcd);

                    rcd.x=0;
                    rcd.y=0;
                    rcd.w=16*stat.cx;
                    rcd.h=12*stat.cy;
                    SDL_SetRenderDrawColor( grenderer, 0xFF, 0xF0, 0xFF, 0x00);
                    SDL_RenderFillRect(grenderer, &rcd);

                    if(stat.main==0){
                        rcd.x=(stat.cx*8)-(stat.wtulisan/2);
                        rcd.y=(stat.cy*6)-(stat.htulisan/2);
                        rcd.w=stat.wtulisan;
                        rcd.h=stat.htulisan;
                        gambar_cacing(grenderer, stat.gambar);
                        gambar_makanan(grenderer, stat.gambar);
                        SDL_RenderCopy(grenderer, stat.kalah, 0, &rcd);
                    }else{
                        gambar_cacing(grenderer, stat.gambar);
                        gambar_makanan(grenderer, stat.gambar);
                    }

                    if(SDL_GetTicks()>wberikutnya){
                        gerak();
                        wberikutnya=SDL_GetTicks()+300;
                    }
                    SDL_RenderPresent( grenderer );
                }

                SDL_DestroyTexture(stat.gambar);
                SDL_DestroyTexture(stat.kalah);

                Mix_PauseMusic();
                Mix_CloseAudio();
                Mix_FreeMusic(musik);
                Mix_FreeChunk(suara);
                Mix_CloseAudio();
                IMG_Quit();
            }
            SDL_DestroyRenderer( grenderer );
            SDL_DestroyWindow( window );
		}
	}

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

int utama(SDL_Event *evt){
    switch(evt->type){
        case SDL_MOUSEBUTTONUP:
            if(stat.main==0)mulai_game();
            break;
        case SDL_KEYDOWN:
            if(stat.main==0 && evt->key.keysym.sym==SDLK_RETURN){
                mulai_game();
            }else stat.tombol=evt->key.keysym.sym;
            break;
        default:
            break;
    }
    return 0;
}

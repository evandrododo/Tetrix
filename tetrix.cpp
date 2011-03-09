/*
Tetrix Project - O Tédio te fazendo um melhor programador
*/

#include <iostream>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

using namespace std;

const int FRAMES_PER_SECOND = 20; 
SDL_Surface *screen = NULL; //Tela principal
TTF_Font *font = NULL;
SDL_Surface *message = NULL, *background;
SDL_Event event;
SDL_Color textColor = {255, 255, 255};


class Timer //totalmente chupada do lazyfoo.net
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}
void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}
void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}
void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}
int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}
bool Timer::is_started()
{
    return started;
}
bool Timer::is_paused()
{
    return paused;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) 
{ 
  SDL_Rect offset; //Temporary rectangle to hold the offsets 
  offset.x = x;//Get the offsets 
  offset.y = y;
  SDL_BlitSurface( source, NULL, destination, &offset );//Blit the surface 
} 

bool init()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return false; } //starta SDL
  screen = SDL_SetVideoMode( 700, 500, 32, SDL_SWSURFACE ); 	//abre janela
  if( screen == NULL ) { return false; } 			//testa janela
  if( TTF_Init() == -1 ) { return false; } 			//starta fonte
  font = TTF_OpenFont( "verdana.ttf", 14 ); 			//seta fonte
  if( font == NULL ) { return false; } 				//testa fonte
  SDL_WM_SetCaption( "Tetrix Project", NULL );			//seta caption
   
  return true;
}

void exit() 
{
  SDL_FreeSurface(message); 	//limpa mensagem
  
  TTF_CloseFont(font);		//limpa fonte
  TTF_Quit();			//Quit TTF
  SDL_Quit();			//Quit SDL 
}

int main( int argc, char* args[] ) 
{
  int frame = 0;
  bool quit=false;
  int m[18][32]; //o que aparece é [18][29], as linhas a mais são para a peça aparecer devagar
  int p1[2],p2[2],p3[2],p4[2],tipo_peca;
  Timer fps;//Tratamento de frames pra não travar tudo
  
  for(int i=0;i<18;i++)
  {
    for(int j=0;j<32;j++)
    {
      m[i][j]=0;
    }
  }
    
  if(init() == false) { return 1; }  
  if(SDL_Flip( screen ) == -1) { return 1; }
  
  background=SDL_LoadBMP("./gameareabg1.bmp");
  
  while(quit==false) //programa em si
  {
    fps.start(); //começa a contar o tempo
    
    apply_surface( 200, 10, background, screen ); 
    
    tipo_peca=1; //colocar random aqui
    switch(tipo_peca)
    {
      //p[0] = x; p[1] = y;
      case 1: p1[0]=8;p1[1]=2;p2[0]=9;p2[1]=2;p3[0]=8;p3[1]=3;p4[0]=9;p4[1]=3;break;// quadrado
      case 2: p1[0]=8;p1[1]=1;p2[0]=8;p2[1]=2;p3[0]=8;p3[1]=3;p4[0]=9;p4[1]=3;break;// L
      case 3: p1[0]=9;p1[1]=1;p2[0]=9;p2[1]=2;p3[0]=9;p3[1]=3;p4[0]=8;p4[1]=3;break;// L invertido
      case 4: p1[0]=8;p1[1]=2;p2[0]=7;p2[1]=3;p3[0]=8;p3[1]=3;p4[0]=9;p4[1]=3;break;// T de ponta cabeça
      case 5: p1[0]=8;p1[1]=1;p2[0]=8;p2[1]=2;p3[0]=9;p3[1]=2;p4[0]=9;p4[1]=3;break;// S
      case 6: p1[0]=9;p1[1]=1;p2[0]=9;p2[1]=2;p3[0]=8;p3[1]=2;p4[0]=8;p4[1]=3;break;// S invertido
      case 7: p1[0]=9;p1[1]=0;p2[0]=9;p2[1]=1;p3[0]=9;p3[1]=2;p4[0]=9;p4[1]=3;break;// LINEPICE
    }
    while(SDL_PollEvent(&event)) 
    {
      if(event.type==SDL_KEYDOWN)
      {
	switch( event.key.keysym.sym ) //trata eventos do teclado
	{ 
	  case SDLK_ESCAPE: quit = true; break;
	}
      }
      
      if((frame%20)==0)
      {
	m[p1[0]][p1[1]]=0;
	m[p2[0]][p2[1]]=0;
	m[p3[0]][p3[1]]=0;
	m[p4[0]][p4[1]]=0;
	p1[0]++;p2[0]++;p3[0]++;p4[0]++;
      }
      m[p1[0]][p1[1]]=tipo_peca;
      m[p2[0]][p2[1]]=tipo_peca;
      m[p3[0]][p3[1]]=tipo_peca;
      m[p4[0]][p4[1]]=tipo_peca;
      
      if(event.type == SDL_QUIT) {quit = true;} //caso clique no X para fechar
	
	
      if( SDL_Flip(screen) == -1 ) { return 1; }
      frame++; 
      if( fps.get_ticks() < (1000/FRAMES_PER_SECOND)) { 
	SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() ); 
      } // sempre junto com o Flip
    }
    
  }
  
  exit();
  
  return 0;
}
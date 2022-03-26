
#include <iostream>
#include <SDL.h>

using namespace std;
////Screen dimension constants
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
//int main( int argc, char* args[] )
//{
//    //The window we'll be rendering to
//    SDL_Window* window = NULL;
//
//    //The surface contained by the window
//    SDL_Surface* screenSurface = NULL;
//
//    //Initialize SDL
//    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
//    {
//        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
//    }
//    else
//    {
//        //Create window
//        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
//        if( window == NULL )
//        {
//            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
//        }
//        else
//        {
//            //Get window surface
//            screenSurface = SDL_GetWindowSurface( window );
//
//            //Fill the surface white
//            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
//
//            //Update the surface
//            SDL_UpdateWindowSurface( window );
//
//            //Wait two seconds
//            SDL_Delay( 2000 );
//        }
//    }
//    //Destroy window
//    SDL_DestroyWindow( window );
//
//    //Quit SDL subsystems
//    SDL_Quit();
//
//    return 0;
//}
VOID LOGSDLERROR(STD::OSTREAM& OS,
                 CONST STD::STRING &MSG, BOOL FATAL = FALSE);

VOID LOGSDLERROR(STD::OSTREAM& OS,
                 CONST STD::STRING &MSG, BOOL FATAL)
{
    OS << MSG << " ERROR: " << SDL_GETERROR() << STD::ENDL;
    IF (FATAL) {
        SDL_QUIT();
        EXIT(1);
    }
}
CONST INT SCREEN_WIDTH = 800;
CONST INT SCREEN_HEIGHT = 600;
CONST STRING WINDOW_TITLE = "AN IMPLEMENTATION OF CODE.ORG PAINTER";

VOID INITSDL(SDL_WINDOW* &WINDOW, SDL_RENDERER* &RENDERER);

VOID INITSDL(SDL_WINDOW* &WINDOW, SDL_RENDERER* &RENDERER)
{
    IF (SDL_INIT(SDL_INIT_EVERYTHING) != 0)
        LOGSDLERROR(STD::COUT, "SDL_INIT", TRUE);

    WINDOW = SDL_CREATEWINDOW(WINDOW_TITLE.C_STR(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //WINDOW = SDL_CREATEWINDOW(WINDOW_TITLE.C_STR(), SDL_WINDOWPOS_CENTERED,
       //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    IF (WINDOW == NULLPTR) LOGSDLERROR(STD::COUT, "CREATEWINDOW", TRUE);


    //KHI CHạY TRONG MôI TRườNG BìNH THườNG (KHôNG CHạY TRONG MáY ảO)
    RENDERER = SDL_CREATERENDERER(WINDOW, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //KHI CHạY ở MáY ảO (Ví Dụ TạI MáY TíNH TRONG PHòNG THựC HàNH ở TRườNG)
    //RENDERER = SDL_CREATESOFTWARERENDERER(SDL_GETWINDOWSURFACE(WINDOW));

    IF (RENDERER == NULLPTR) LOGSDLERROR(STD::COUT, "CREATERENDERER", TRUE);

    SDL_SETHINT(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");
    SDL_RENDERSETLOGICALSIZE(RENDERER, SCREEN_WIDTH, SCREEN_HEIGHT);
}

VOID QUITSDL(SDL_WINDOW* WINDOW, SDL_RENDERER* RENDERER);

VOID QUITSDL(SDL_WINDOW* WINDOW, SDL_RENDERER* RENDERER)
{
	SDL_DESTROYRENDERER(RENDERER);
	SDL_DESTROYWINDOW(WINDOW);
	SDL_QUIT();
}

VOID WAITUNTILKEYPRESSED();

VOID WAITUNTILKEYPRESSED()
{
    SDL_EVENT E;
    WHILE (TRUE) {
        IF ( SDL_WAITEVENT(&E) != 0 &&
             (E.TYPE == SDL_KEYDOWN || E.TYPE == SDL_QUIT) )
            RETURN;
        SDL_DELAY(100);
    }
}

INT MAIN(INT ARGC, CHAR* ARGV[])
{
    SDL_WINDOW* WINDOW;
    SDL_RENDERER* RENDERER;
    INITSDL(WINDOW, RENDERER);

    // YOUR DRAWING CODE HERE
    // USE SDL_RENDERPRESENT(RENDERER) TO SHOW IT
    SDL_RENDERCLEAR(RENDERER);

    SDL_SETRENDERDRAWCOLOR(RENDERER, 255, 255, 255, 255);   // WHITE
    SDL_RENDERDRAWPOINT(RENDERER, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SDL_SETRENDERDRAWCOLOR(RENDERER, 255, 0, 0, 255);   // RED
    SDL_RENDERDRAWLINE(RENDERER, 100, 100, 200, 200);
    SDL_RECT FILLED_RECT;
    FILLED_RECT.X = SCREEN_WIDTH - 400;
    FILLED_RECT.Y = SCREEN_HEIGHT - 150;
    FILLED_RECT.W = 320;
    FILLED_RECT.H = 100;
    SDL_SETRENDERDRAWCOLOR(RENDERER, 0, 255, 0, 255); // GREEN
    SDL_RENDERFILLRECT(RENDERER, &FILLED_RECT);


   //KHI THôNG THườNG CHạY VớI MôI TRườNG BìNH THườNG ở NHà
    SDL_RENDERPRESENT(RENDERER);
   //KHI CHạY ở MáY THựC HàNH WINXP ở TRườNG (MáY ảO)
   //SDL_UPDATEWINDOWSURFACE(WINDOW);


    WAITUNTILKEYPRESSED();
    QUITSDL(WINDOW, RENDERER);
    RETURN 0;
}




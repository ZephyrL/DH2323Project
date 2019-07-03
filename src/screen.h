#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <glm/glm.hpp>
#include <string>
#include <SDL.h>

using namespace glm;
using namespace std;

class Camera; // extern

const int DEFAULT_WIDTH = 600;
const int DEFAULT_HEIGHT = 480;

class Screen
{
    friend class Camera;

private:
    int width, height;
    int timer; // fps timer
    SDL_Surface *screen;

private:
    void fps();

    // Initializes SDL (video and timer). SDL creates a window where you can draw.
    // A pointer to this SDL_Surface is returned. After calling this function
    // you can use the function PutPixelSDL to do the actual drawing.
    SDL_Surface *InitializeSDL(int width, int height, bool fullscreen = false);

    // Checks all events/messages sent to the SDL program and returns true as long
    // as no quit event has been received.
    bool NoQuitMessageSDL();

    // Draw a pixel on a SDL_Surface. The color is represented by a glm:vec3 which
    // specifies the red, green and blue component with numbers between zero and
    // one. Before calling this function you should call:
    // if( SDL_MUSTLOCK( surface ) )
    //     SDL_LockSurface( surface );
    // After calling this function you should call:
    // if( SDL_MUSTLOCK( surface ) )
    //     SDL_UnlockSurface( surface );
    // SDL_UpdateRect( surface, 0, 0, 0, 0 );
    void PutPixelSDL(SDL_Surface *surface, int x, int y, glm::vec3 color);

public:
    inline void put(int x, int y, vec3 color);

    inline void save(const string &name);

public:
    Screen(int w = DEFAULT_WIDTH, int h = DEFAULT_HEIGHT) : width(w), height(h)
    {
        screen = InitializeSDL(width, height);
        timer = SDL_GetTicks();
    }

    inline bool NotQuit();

    ~Screen()
    {
    }
};

inline void Screen::put(int x, int y, vec3 color)
{
    return PutPixelSDL(screen, x, y, color);
}

inline bool Screen::NotQuit()
{
    return NoQuitMessageSDL();
}

inline void Screen::save(const string &name)
{
    SDL_SaveBMP(screen, name.c_str());
}

#endif // !_SCREEN_H_
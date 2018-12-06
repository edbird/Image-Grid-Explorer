
// local headers
#include "IndexFileProcessor.hpp"
#include "ErrorLog.hpp"


// SDL headers
#include <SDL.h>


// C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>






// TODO: check that images are addressed in sequential order starting from zero with no gaps
// TODO: change window title depending on address


int main()
{


    ErrorLog errorlog("error.log");


    IndexFileProcessor p("index.txt", errorlog);
    if(!p.Ok())
    {
        return -1;
    }
    else
    {

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "SDL_Init failure" << SDL_GetError() << std::endl;
            return -2;
        }

        // get window size
        int size_x{640};
        int size_y{480};
        p.SetWindowSize(size_x, size_y);

        // create window
        SDL_Window *window{SDL_CreateWindow("Image Grid Explorer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size_x, size_y, SDL_WINDOW_SHOWN)};
        if(window == nullptr)
        {
            std::cout << "SDL_CreateWindow failure: " << SDL_GetError() << std::endl;
            return -3;
        }

        // get dimension
        int dimension{0};
        p.SetDimension(dimension);
        if(dimension < 1)
        {
            return -5;
        }
        Address address(dimension);
        Address address_prev(address);

        // load initial image
        std::cout << "loading initial image" << std::endl;
        SDL_Surface *screen_surface;
        screen_surface = SDL_GetWindowSurface(window);
        SDL_Surface *image_surface;
            
        // blit image to screen
        if(p.ImageIndex().size() < 1)
        {
            return -4;
        }
        std::string image_filename{p.ImageIndex().begin()->first};
        image_surface = SDL_LoadBMP(image_filename.c_str());
        if(image_surface == nullptr)
        {
            std::cout << "SDL_LoadBMP failure: " << SDL_GetError() << std::endl;
            // TODO: use error output class
        }
        
        SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
        SDL_UpdateWindowSurface(window);


        bool quit{false};
        SDL_Event event;
        while(!quit)
        {

            // blit image to screen
            if(p.ImageIndex().size() < 1)
            {
                return -4;
            }
            if(address != address_prev)
            {
                for(std::map<std::string, Address>::const_iterator it{p.ImageIndex().begin()};
                    it != p.ImageIndex().end(); ++ it)
                {
                    if(it->second == address)
                    {
                        std::string image_filename{it->first};
                        image_surface = SDL_LoadBMP(image_filename.c_str());
                        if(image_surface == nullptr)
                        {
                            std::cout << "SDL_LoadBMP failure: " << SDL_GetError() << std::endl;
                            // TODO: use error output class
                        }
                        address_prev = address;
                        
                        break;
                    }
                }
            
            }

            SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
            SDL_UpdateWindowSurface(window);
            
        
            while(SDL_PollEvent(&event) != 0)
            {
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if(event.type == SDL_KEYDOWN)
                {

                    SDL_Keycode keycode{event.key.keysym.sym};
                    SDL_Keymod keymod{SDL_GetModState()};

                    if(keycode == SDLK_q && ((keymod & KMOD_LCTRL) > 0))
                    {
                        quit = true;
                    }
                    else if(keycode == SDLK_z || keycode == SDLK_x)
                    {
                        int dimension_index = 0;

                        if(keycode == SDLK_z)
                        {
                            if(address.Dimension() >= 1)
                            {
                                if(address.At(dimension_index) > 0)
                                {
                                    address.Decr(dimension_index);
                                }
                            }
                        }

                        if(keycode == SDLK_x)
                        {
                            if(address.Dimension() >= 1)
                            {
                                if(address.At(dimension_index) < 2) // TODO:
                                {
                                    address.Incr(dimension_index);
                                }
                            }
                        }

                        std::cout << "Address: " << address << std::endl;

                    }
                    else if(keycode == SDLK_a || keycode == SDLK_s)
                    {
                        int dimension_index = 1;

                        if(keycode == SDLK_a)
                        {
                            if(address.Dimension() >= 2)
                            {
                                if(address.At(dimension_index) > 0)
                                {
                                    address.Decr(dimension_index);
                                }
                            }
                        }

                        if(keycode == SDLK_s)
                        {
                            if(address.Dimension() >= 2)
                            {
                                if(address.At(dimension_index) < 2) // TODO:
                                {
                                    address.Incr(dimension_index);
                                }
                            }
                        }

                        std::cout << "Address: " << address << std::endl;

                    }
                    else if(keycode == SDLK_q || keycode == SDLK_w)
                    {
                        int dimension_index = 2;

                        if(keycode == SDLK_q)
                        {
                            if(address.Dimension() >= 3)
                            {
                                if(address.At(dimension_index) > 0)
                                {
                                    address.Decr(dimension_index);
                                }
                            }
                        }

                        if(keycode == SDLK_w)
                        {
                            if(address.Dimension() >= 3)
                            {
                                if(address.At(dimension_index) < 2) // TODO:
                                {
                                    address.Incr(dimension_index);
                                }
                            }
                        }

                        std::cout << "Address: " << address << std::endl;

                    }
                    else
                    {

                    }


                }
            }
        }

        SDL_DestroyWindow(window);

        SDL_Quit();

        return 0;

    }

}

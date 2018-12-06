
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


class ErrorLog
{

    public:

    ErrorLog(const std::string& filename)
    {
        error_log.open(filename.c_str(), std::ios::trunc);
    }


    virtual
    ~ErrorLog()
    {
        error_log.flush();
        error_log.close();
    }


    void Print(const std::string& message)
    {
        error_log << message << std::endl;
    }



    protected:

    std::ofstream error_log;

};




// TODO: check that images are addressed in sequential order starting from zero with no gaps



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

        int size_x{640};
        int size_y{480};
        p.SetWindowSize(size_x, size_y);

        SDL_Window *window{SDL_CreateWindow("Image Grid Explorer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size_x, size_y, SDL_WINDOW_SHOWN)};
        if(window == nullptr)
        {
            std::cout << "SDL_CreateWindow failure: " << SDL_GetError() << std::endl;
            return -3;
        }

        int dimension{0};
        p.SetDimension(dimension);
        if(dimension < 1)
        {
            return -5;
        }
        std::vector<int> address;
        std::vector<int> address_prev;
        for(int c{0}; c < dimension; ++ c)
        {
            address.push_back(0);
        }

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
        
        address_prev = address;
        
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
                for(std::map<std::string, std::vector<int>>::const_iterator it{p.ImageIndex().begin()};
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

                    int dimension_index{0};

                    switch(event.key.keysym.sym)
                    {
                        case SDLK_z:

                            dimension_index = 0;
                            if(dimension >= 1)
                            {
                                if(address.at(dimension_index) > 0)
                                {
                                    -- address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;


                        case SDLK_x:

                            dimension_index = 0;
                            if(dimension >= 1)
                            {
                                if(address.at(dimension_index) < 2) // TODO:
                                {
                                    ++ address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;

                        
                        case SDLK_a:

                            dimension_index = 1;
                            if(dimension >= 2)
                            {
                                if(address.at(dimension_index) > 0)
                                {
                                    -- address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;


                        case SDLK_s:

                            dimension_index = 1;
                            if(dimension >= 2)
                            {
                                if(address.at(dimension_index) < 2) // TODO:
                                {
                                    ++ address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;

                        
                        case SDLK_q:

                            dimension_index = 2;
                            if(dimension >= 3)
                            {
                                if(address.at(dimension_index) > 0)
                                {
                                    -- address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;


                        case SDLK_w:

                            dimension_index = 2;
                            if(dimension >= 3)
                            {
                                if(address.at(dimension_index) < 2) // TODO:
                                {
                                    ++ address.at(dimension_index);
                                }
                            }

                            std::cout << "Address: ";
                            for(int c{0}; c < dimension; ++ c)
                            {
                                std::cout << address.at(c) << " ";
                            }
                            std::cout << std::endl;

                            break;


                    }

                }
            }
        }

        SDL_DestroyWindow(window);

        SDL_Quit();

        return 0;

    }

}

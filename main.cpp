
// local headers, internal
#include "IndexFileProcessor.hpp"
#include "ErrorLog.hpp"


// local headers, external
#include "program_arguments.hpp"


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


void print_arg_help(std::ostream& os)
{
    os << "Image Grid Explorer\n"\
       << "\n"\
       << "Arguments:\n"\
       << "\n"\
       << "     --help\n"\
       << " Print help\n"\
       //<< "     --filename [FILENAME] [DIM1] <DIM2> <DIM3>\n"
       //<< " Generate index.txt file"
       << "     --generate [DIM1] <DIM2> <DIM3>\n"\
       << " Generate index.txt file with dimensions DIM1 DIM2 DIM3\n"\
       << "";
}


int main(int argc, char* argv[])
{


    ////////////////////////////////////////////////////////////////////////////
    // PROCESS PROGRAM ARGUMENTS
    ////////////////////////////////////////////////////////////////////////////

    // new program arguments processing method
    ProgramArguments pa;
    pa.Add("help", "--help", "false");
    //pa.Add("filename", "--filename", "index.txt");
    pa.Add("generate", "--generate", "1");
    pa.Print();
    pa.Process(argc, argv);

    if(pa.Get("help") != std::string("false"))
    {
        print_arg_help(std::cout);
    }

    if(pa.WasProvided("generate"))
    {
        std::string dimensions_string{pa.Get("generate")};
        std::vector<int> dimensions;
        std::size_t search_ix{0};
        std::size_t next_search_ix{0};

        while((next_search_ix = dimensions_string.find(" ", search_ix)) != std::string::npos)
        {
            std::cout << "..." << std::endl;
            std::cout << next_search_ix << std::endl;
            std::cout << search_ix << std::endl;
            std::string next_dimension_string{dimensions_string.substr(search_ix, next_search_ix - search_ix)};
            std::cout << next_dimension_string << std::endl;
            dimensions.push_back(std::stoi(next_dimension_string));
            search_ix = next_search_ix;
        }
    }

    // init error log
    ErrorLog errorlog("error.log");


    // process index.txt file
    IndexFileProcessor p("index.txt", errorlog);
    if(!p.Ok())
    {
        return -1;
    }
    else
    {

        // init SDL
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


        // main loop

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
            

            // process events
        
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


        // clean

        SDL_DestroyWindow(window);

        SDL_Quit();

        return 0;

    }

}

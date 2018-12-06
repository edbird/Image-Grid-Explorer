

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


class IndexFileProcessor
{

    public:

    IndexFileProcessor(const std::string& filename, ErrorLog& errorlog)
        : ok{true}
        , errorlog(errorlog)
        , dimension_set{false}
        , window_size_x_set{false}
        , window_size_y_set{false}
    {
        indexfile.open("index.txt");
        if(!indexfile.is_open())
        {
            errorlog.Print("Error: Could not open file index.txt");
            ok = false;
        }
        else
        {

            // process file contents
            std::string line;
            while(std::getline(indexfile, line))
            {
                if(line.find("=") != std::string::npos)
                {
                    // = was found
                    std::size_t position{line.find("=")};
                    std::string string1{line.substr(0, position)};
                    std::string string2{line.substr(position + 1)};
                    std::cout << string1 << "," << string2 << std::endl;

                    if(string1 == std::string("dimension"))
                    {
                        dimension = std::stoi(string2);
                        dimension_set = true;
                        std::cout << "SET dimension: " << dimension << std::endl;
                    }
                    else
                    {
                        std::cout << "? (1)" << std::endl;
                    }
                }
                else
                {
                    // = not found
                    if(line.find(" ") != std::string::npos)
                    {
                        // space was found
                        std::size_t position{line.find(" ")};
                        std::string instruction{line.substr(0, position)};
                        if(instruction == std::string("keymap"))
                        {
                            // TODO
                        }
                        else if(instruction == std::string("window"))
                        {
                            std::string temp{line.substr(position + 1)};
                            if(temp.find(" ") != std::string::npos)
                            {                            
                                std::string string_x{temp.substr(0, temp.find(" "))};
                                std::string string_y{temp.substr(temp.find(" ") + 1)};
                                window_size_x = std::stoi(string_x);
                                window_size_y = std::stoi(string_y);
                                if(window_size_x > 0)
                                {
                                    window_size_x_set = true;
                                }
                                if(window_size_y > 0)
                                {
                                    window_size_y_set = true;
                                }
                            }
                            else
                            {
                                std::cout << "? (3)" << std::endl;
                            }
                        }
                        //else if(instruction == std::string("#"))
                        else if(instruction.size() > 0 && (instruction.at(0) == '#'))
                        {
                            // comment, ignore
                        }
                        else
                        {
                            // something else, could be an image index specifier (filename and address)
                            // instruction is the filename
                            std::ifstream imagefile(instruction.c_str());
                            if(imagefile.is_open())
                            {
                                // file exists
                                imagefile.close();

                                std::string temp{line.substr(position + 1)};
                                // int dimension_counter{0}; this is the same as image_address.size()
                                std::vector<int> image_address;
                                while(temp.size() > 0)
                                {
                                    if(temp.find(" ") != std::string::npos)
                                    {
                                        std::string address_string{temp.substr(0, temp.find(" "))};
                                        std::cout << "address_string=" << address_string << "." << std::endl;
                                        int address_int{std::stoi(address_string)};
                                        if(address_int >= 0)
                                        {
                                            image_address.push_back(address_int);
                                        }
                                        temp = temp.substr(temp.find(" ") + 1);
                                    }
                                    else
                                    {
                                        //std::string address_string{temp.substr(0, temp.find(" "))};
                                        std::string address_string{temp};
                                        std::cout << "address_string=" << address_string << "." << std::endl;
                                        int address_int{std::stoi(address_string)};
                                        if(address_int >= 0)
                                        {
                                            image_address.push_back(address_int);
                                        }
                                        // temp = temp.substr(temp.find(" ") + 1);
                                        temp.clear(); // break infinite loop
                                    }
                                }
                                std::cout << "finished processing address" << std::endl;

                                if(image_address.size() == dimension)
                                {
                                    // TODO: check on return value for success
                                    image_index.insert({instruction, image_address});
                                }
                                else
                                {
                                    std::cout << "? (5)" << std::endl;
                                }
                            }
                            else
                            {
                                std::cout << "? (4)" << std::endl;
                            }
                        }
                    }
                    else
                    {
                        // space was not found
                        std::cout << "? (2)" << std::endl;
                    }

                }
            }
        }
        
    }

    bool Ok()
    {
        return ok;
    }

    const std::map<std::string, std::vector<int>>& ImageIndex()
    {
        return image_index;
    }

    void SetWindowSize(int &size_x, int &size_y)
    {
        if(window_size_x_set && window_size_y_set)
        {
            size_x = window_size_x;
            size_y = window_size_y;
        }
        else
        {
            errorlog.Print("Warning: Window size x and y not set");
        }
    }

    void SetDimension(int &dimension)
    {
        if(dimension_set)
        {
            dimension = this->dimension;
        }
        else
        {
            errorlog.Print("Warning: Dimension not set");
        }
    }

    protected:

    std::ifstream indexfile;
    bool ok;
    ErrorLog& errorlog;

    // properties
    int dimension;
    bool dimension_set;
    std::map<std::string, std::vector<int>> image_index;
    std::vector<std::string> keymap_instructions;
    int window_size_x;
    bool window_size_x_set;
    int window_size_y;
    bool window_size_y_set;

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

#ifndef INDEX_FILE_PROCESSOR_HPP
#define INDEX_FILE_PROCESSOR_HPP


// local headers
#include "Address.hpp"
#include "ErrorLog.hpp"


// C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>


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
                                    Address image_address_2(dimension);
                                    image_address_2.SetAddress(image_address);
                                    // TODO: check on return value for success
                                    image_index.insert({instruction, image_address_2});
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

    const std::map<std::string, Address>& ImageIndex()
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
    std::map<std::string, Address> image_index;
    std::vector<std::string> keymap_instructions;
    int window_size_x;
    bool window_size_x_set;
    int window_size_y;
    bool window_size_y_set;

};

#endif // INDEX_FILE_PROCESSOR_HPP

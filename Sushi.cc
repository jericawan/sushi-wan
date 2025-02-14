#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include "Sushi.hh"
//worked with Qian Qian and Damir on understanding instructions
std::string Sushi::read_line(std::istream &in)
{
  std::string line;
  if(!std::getline (in, line)) {// Has the operation failed?
    if(!in.eof()) { 
      std::perror("getline");
    }
    return "";
  }
    
  // Is the line empty?
  if(std::all_of(line.begin(), line.end(), isspace)) {
    return "";
  }

  // Is the line too long?
  if(line.size() > MAX_INPUT_SIZE) {
    line.resize(MAX_INPUT_SIZE);
    std::cerr << "Line too long, truncated." << std::endl;
  }
  
  return line; 
}

bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  // Try to open a config file
  std::ifstream config_file(fname);
  if (!config_file) {
    if (!ok_if_missing) {
      std::perror(fname);
      return false;
    }
    return true;
  }

  // Read the config file
  while(!config_file.eof()) {
    std::string line = read_line(config_file);
    //tests line with parse command 
    if(Sushi::parse_command(line)==0)
    { //has syntax errors 
      continue;
    }
    else
    {
      store_to_history(line);
    }
  }
  return true; 
}

void Sushi::store_to_history(std::string line)
{
  if (line.empty()) {
    return;    
  }

  // Is the history buffer full?
  while (history.size() >= HISTORY_LENGTH) {
    history.pop_front();
  }
  
  history.push_back(line);
}

void Sushi::show_history() const
{
  int index = 1;
  for (const auto &cmd: history) {
    std::cout << std::setw(5) << index++ << "  " << cmd << std::endl;
  }
}

void Sushi::set_exit_flag()
{
  bool temp=get_exit_flag();
  if(temp==true)
  {
    exit_flag=false;
  }
  else
  {
    exit_flag=true;
  }

}

bool Sushi::get_exit_flag() const
{
  return exit_flag; 
}

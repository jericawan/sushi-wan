#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <cstring> // DZ: For strdup
#include "Sushi.hh"
#include <vector>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
//received help from Damir and Qian Qian
//unable to do dir but able to do gdir 
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
    if(!parse_command(line)) {
      store_to_history(line);
    }
  }
  
  return true; 
}

void Sushi::store_to_history(std::string line)
{
  // Do not insert empty lines
  if (line.empty()) {
    return;    
  }

  // Is the history buffer full?
  while (history.size() >= HISTORY_LENGTH) {
    history.pop_front();
  }
  
  history.push_back(line);
}

void Sushi::show_history() 
{
  int index = 1;

  // `history` itself will be inserted
  if (history.size() == HISTORY_LENGTH) {
    history.pop_front();
  }
  
  for (const auto &cmd: history) {
    std::cout << std::setw(5) << index++ << "  " << cmd << std::endl;
  }
  
  // `history` itself will be inserted
  std::cout << std::setw(5) << index++ << "  " << "history" << std::endl;
}

void Sushi::set_exit_flag()
{
  exit_flag = true;
}

bool Sushi::get_exit_flag() const
{
  return exit_flag;
}

//---------------------------------------------------------
// New methods
int Sushi::spawn(Program *exe, bool bg)
{
  UNUSED(bg);
  pid_t system_process = fork();
  if (system_process == -1) {
    std::perror("error with fork");
    return EXIT_FAILURE;
  }
  else if(system_process == 0)
  {
    //convert to array
    // DZ -- ???
    // char* const* arrayB=exe->vector2arrayPublic();
    char* const* arrayB=exe->vector2array();
    if(execvp(exe->progname().c_str(), arrayB)==-1)
    {
      // DZ: Error message???
      std::perror(arrayB[0]);
      exit(EXIT_FAILURE);
    }
  } 
  else
  {
    int status;
    if(waitpid(system_process,&status,0)==-1)
    {
      // DZ: Use perror to report errors
      // std::cerr <<"error"<<std::endl;
      std::perror("waitpid");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  // DZ: Use perror to report errors
  // std::perror("error with fork");
  std::perror("fork");  
  return EXIT_FAILURE;
}

void Sushi::prevent_interruption() {
  //declare sigaction var 
  struct sigaction sigAct;
  sigAct.sa_handler = refuse_to_die;
  sigemptyset(&sigAct.sa_mask);
  sigAct.sa_flags = SA_RESTART;

  sigaction(SIGINT, &sigAct, NULL);
}

void Sushi::refuse_to_die(int signo) {

  // DZ: Wrong message
  // std::cerr << "type exit to shell" << std::endl;
  std::cerr << "Type exit to exit the shell\n";
  UNUSED(signo);
}

void Sushi::mainloop() {
  // Must be implemented
}

char* const* Program::vector2array() {
    size_t size = args->size();
    char** arr = new char*[size + 1];

    for (size_t i = 0; i < size; i++) {
      // DZ: Wrong, there is no need to duplicate
        arr[i] = strdup(args->at(i)->c_str()); // strdup ensures valid memory allocation
    }
    arr[size] = nullptr;
    return arr;
}
void Program::free_array(char *const argv[]) 
{
  if(argv==nullptr){
    return;
  }
  int i=0;
  while(argv[i]!=nullptr)
  {
    // DZ: Do not duplicate, do not free
    //free dunamically allocated mem for each element
    // DZ: It is an error to use delete after strdup()
    delete argv[i];
    i++;
  }
  //free array as a whole
  delete argv;
}
void Program:: name(char *const argv[]) 
{

  free_array(argv);
}

Program::~Program() {
  // Do not implement now
}

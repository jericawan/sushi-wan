#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

// DZ: Do NOT
// using namespace std;
// Initialize the static constants
const size_t Sushi::MAX_INPUT_SIZE = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
int Sushi::count=1;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

Sushi my_shell; // New global var

int main(int argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  // DZ: Moved to globals; not an error  
  //Sushi sushi;
  
  // test  
  std::string line;
  // DZ: YOUR home is not MY home. Our homes are defined in the $HOME environment variable.
  // DZ: Returned value must be checked and used.
  my_shell.read_config("/Users/jericawan/Desktop/sushi-wan2/sushi-wan/sushi.conf",true);
  my_shell.show_history();
  // DZ: Remove dead code before submitting
  // //file dne
  // cout<<"testing 1:"<<endl;
  // my_shell.read_config("sushii.conf",false);
  while(true) {
    std::cout << Sushi::DEFAULT_PROMPT;
    line = my_shell.read_line(std::cin);
    // DZ: Must insert into history here
     my_shell.show_history();
  }
  return EXIT_SUCCESS;
}


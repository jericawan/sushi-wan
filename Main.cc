#include <cstdlib>
#include "Sushi.hh"

// Initialize the static constants
Sushi my_shell; 
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";
const std::string Sushi::DEFAULT_CONFIG = "sushi.conf";

int main(int argc, char *argv[])
{

  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  for(int i=1; i<argc; i++)
  {
    const char* script=argv[i];
    //checks if script exists/has errors 
    if(!my_shell.read_config(script,false))
    {
      std::cerr << "Error reading script: " << script << std::endl;
      return EXIT_FAILURE;
    }
  }
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  my_shell.mainloop();
  
  return EXIT_SUCCESS;
}

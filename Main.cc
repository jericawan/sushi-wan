#include <cstdlib>
#include "Sushi.hh"

// Initialize the static constants
Sushi my_shell; // New global var
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";
const std::string Sushi::DEFAULT_CONFIG = "sushi.conf";


int main(int argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  // New function call
  Sushi::prevent_interruption();
  
  const char *home_dir = std::getenv("HOME");
  if (!home_dir) {
    std::cerr << "Error: HOME environment variable not set." << std::endl;
    return EXIT_FAILURE;
  }
  std::string config_path = std::string(home_dir) + "/" + Sushi::DEFAULT_CONFIG;
  // OK if missing!
  my_shell.read_config(config_path.c_str(), true);

  // std::cout << Sushi::DEFAULT_PROMPT;
  // std::string command = Sushi::read_line(std::cin);
  //while loop 
  my_shell.set_exit_flag();
  while(my_shell.get_exit_flag()==true)
  {
      std::cout << Sushi::DEFAULT_PROMPT;
      std::string command = Sushi::read_line(std::cin);
      Sushi::parse_command(command);
      // DZ: Must check the returned value!
      // if(Sushi::parse_command(command)==0)
      // {
      //   my_shell.store_to_history(command);
      // }
      my_shell.store_to_history(command);
      
  }
  // DZ: Not needed
  // my_shell.show_history();
  
  return EXIT_SUCCESS;


}

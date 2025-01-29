#include <cstdlib>
#include <iostream>
#include "Sushi.hh"
using namespace std;
// Initialize the static constants
const size_t Sushi::MAX_INPUT_SIZE = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
int Sushi::count=1;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";


int main(int argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  // test
  Sushi sushi;
  std::string line;
  sushi.read_config("/Users/jericawan/Desktop/sushi-wan2/sushi-wan/sushi.conf",true);
  sushi.show_history();
  // //file dne
  // cout<<"testing 1:"<<endl;
  // sushi.read_config("sushii.conf",false);
  while(true) {
    std::cout << Sushi::DEFAULT_PROMPT;
    line = sushi.read_line(std::cin);
     sushi.show_history();
  }
  return EXIT_SUCCESS;
}


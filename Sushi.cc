#include <cmath>
#include "Sushi.hh"
// Do NOT
// using namespace std;

std::string Sushi::read_line(std::istream &in)
{
  std::string line;
  // DZ: Must check the returned value
  getline(in,line);
  //check \n
  if(in.fail() && !in.eof())
    {
      // DZ: Wrong use of perror
      // perror("Error");
      std::perror("command line");
      // DZ: Useless
      // in.ignore(numeric_limits<streamsize>::max(), '\n');
        return "";
    }
    //check if it exceeds 
    if(line.size()>Sushi::MAX_INPUT_SIZE)
    {
      // DZ: "line" != "Line"
      std::cerr << "line too long, truncated" << std::endl;
      line=line.substr(0, Sushi::MAX_INPUT_SIZE);
      return "";
    }

    // DZ: Must check for space characters first
    //check if line empty/only whitespace
    if(line.empty())
    {
      // DZ: Needless message
      // std::cerr<<"line is empty"<<std::endl;
      return "";
    }
    // DZ: Not this function's responsibility 
      my_shell.store_to_history(line);
      return line;
}

//read_config
bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  std::ifstream file(fname);
	 if(!file.good())
	{
		if(ok_if_missing!=false)
		{
			return true;
		}
    else
	{
	  // DZ: Wrong use of perror
	  // std::perror("error opening file");
	  std::perror(fname);
		 return false;
	}
	}
	
// while(!read_line(file).empty());
while(!file.eof())
  {
    read_line(file);
    // DZ: Insert into history here
  }
 file.close();
 // DZ: Cannot check the staus of a closed file
 //if(file.fail())
 //{
 //		std::perror("error closing file");
 // 		return false;
 //	}
return true;
}
void Sushi::store_to_history(std::string line)
{
  // DZ: `size_t`, not `int` (unsigned comparison)
  // DZ: The variable never used
  int size=history.size();
  if (line.empty()) 
  {
    return;
  }
 if(history.size()==Sushi::HISTORY_LENGTH)
  {
    history.pop();
    history.push(line); // DZ: Move outside of `if`
  }
else
  {
    history.push(line); // DZ: Move outside of `if`
  }
  count++;
}
void Sushi::show_history()
{
  // DZ: Wrong interpretation of requirements
  // DZ: Each string should be prefixed by the sequential number (starting at 1)
  std::queue<std::string> temp = history;
  // DZ: `size_t`, not `int` (unsigned comparison)
  for (int i = 0; i < history.size(); i++) 
  {
    if(Sushi::count-history.size()<0)
    {
      std::cout << std::setw(5) << std::right << history.size()-Sushi::count+i << " " << temp.front() << std::endl;
    }
    else if(Sushi::count-history.size()>=0)
    {
      std::cout << std::setw(5) << std::right << Sushi::count-history.size()+i << " " << temp.front() << std::endl;
    }
    temp.pop();
    
  }
}

void Sushi::set_exit_flag()
{
  // To be implemented
}

bool Sushi::get_exit_flag() const
{
  return false; // To be fixed
}

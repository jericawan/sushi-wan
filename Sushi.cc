#include <cmath>
#include "Sushi.hh"
using namespace std;

string Sushi::read_line(istream &in)
{
  string line;
  getline(in,line);
  //check \n
  if(in.fail() && !in.eof())
    {
        perror("Error");
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        return "";
    }
    //check if it exceeds 
    if(line.size()>Sushi::MAX_INPUT_SIZE)
    {
      cerr << "line too long, truncated" << endl;
      line=line.substr(0, Sushi::MAX_INPUT_SIZE);
      return "";
    }
    //check if line empty/only whitespace
    if(line.empty())
    {
      cerr<<"line is empty"<<endl;
      return "";
    } 
      store_to_history(line);
      return line;
}

//read_config
bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  ifstream file(fname);
	 if(!file.good())
	{
		if(ok_if_missing!=false)
		{
			return true;
		}
    else
	{
		std::perror("error opening file");
		 return false;
	}
	}
	
// while(!read_line(file).empty());
while(!file.eof())
  {
    read_line(file);
  }
 file.close();
 if(file.fail())
	{
		std::perror("error closing file");
    		return false;
	}
return true;
}
void Sushi::store_to_history(std::string line)
{
  int size=history.size();
  if (line.empty()) 
  {
    return;
  }
 if(history.size()==Sushi::HISTORY_LENGTH)
  {
    history.pop();
    history.push(line);
  }
else
  {
    history.push(line);
  }
  count++;
}
void Sushi::show_history()
{
  std::queue<std::string> temp = history;
  for (int i = 0; i < history.size(); i++) 
  {
    if(Sushi::count-history.size()<0)
    {
      cout << std::setw(5) << std::right << history.size()-Sushi::count+i << " " << temp.front() << std::endl;
    }
    else if(Sushi::count-history.size()>=0)
    {
      cout << std::setw(5) << std::right << Sushi::count-history.size()+i << " " << temp.front() << std::endl;
    }
    temp.pop();
    
  }
}
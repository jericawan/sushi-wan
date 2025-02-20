#include <cstring> // DZ: for strlen
#include "Sushi.hh"
//used chatgpt to explain instructions
std::string *Sushi::unquote_and_dup(const char* s)
{
  std::string result="";
  // DZ: sizeof(s) = sizeof(char*) = 4
  // DZ: You need strlen(s)
  // for(int i=0;i<sizeof(s);i++)
    for(int i=0;i<strlen(s);i++)
	{
    //determines if the character is a backslash
    char t=s[i];
    char backslash='\\';
    if(t==backslash) // DZ: Why not `if(t=='\\')` ? 
		{
		  //character IS backslash 
		  // temp=s[i];
		  // temp+=s[i+1];
		  // DZ: `s[i,i+1]` is just `s[i+1]`
		  // DZ: You probably meant something else
      switch(s[i,i+1])
      {
      case '\a':  // DZ: must be 'a'
          result=result+'\a';
          break;
        case '\b': // DZ: must be 'b'
          result=result+'\b';
          break;
        case '\e': // DZ: must be 'e'
          result=result+'\x1B'; // DZ: '\e' is obsolete
          break;
      case '\f': // DZ: ....
          result=result+'\f';
          break;
        case '\n':
          result=result+'\n';
          break;
        case '\r':
          result=result+'\r';
          break;
        case '\t':
          result=result+'\t';
          break;
        case '\v':
          result=result+'\v';
          break;
        case '\\':
          result=result+'\\';
          break;
        case '\'':
          result=result+'\'';
          break;
        case '\"':
          result=result+'\"';
          break;
      }
		i+=1;
		}
    //not backslash, continue
    else
    {
      result+=s[i];
    }
	//return tempStr;
    // DZ: `return` in the wrong position
    // return new std::string(result); 
}
    return new std::string(result); 
}

void Sushi::re_parse(int i) 
{
  //store or return error
  // DZ: comparison of integer expressions of different signedness
  if(i<=0 || i>static_cast<unsigned>(history.size()))
	{
		std::cerr << "Error: " << i<< " event not found"<<std::endl;
	}
	else
	{
     //find string of i-th command
  std::string command=Sushi::history[i-1];
  //result of parse_command
  int result=Sushi::parse_command(command);
  if(result!=0)
  {
		Sushi::store_to_history(command);
	}
}
}

//---------------------------------------------------------------
// Do not modify this function YET
std::string *Sushi::getenv(const char* s) 
{
  return new std::string(s); // Must be changed - eventually
}

//---------------------------------------------------------------
// Do not modify this function
void yyerror(const char* s) {
  std::cerr << "Parse error: " << s << std::endl;
}


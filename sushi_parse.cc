#include "Sushi.hh"
//used chatgpt to explain instructions
std::string *Sushi::unquote_and_dup(const char* s)
{
  std::string result="";
  for(int i=0;i<sizeof(s);i++)
	{
    //determines if the character is a backslash
    char t=s[i];
    char backslash='\\';
		if(t==backslash)
		{
      //character IS backslash 
			// temp=s[i];
      // temp+=s[i+1];
      switch(s[i,i+1])
      {
        case '\a':
          result=result+'\a';
          break;
        case '\b':
          result=result+'\b';
          break;
        case '\e':
          result=result+'\e';
          break;
        case '\f':
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
  return new std::string(result); // Must be changed
}
}

void Sushi::re_parse(int i) 
{
  //store or return error 
	if(i<=0 || i>history.size())
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


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <exception>
#include <boost/timer/timer.hpp>
#include <sys/resource.h>
#include "procx.h"

using namespace std;
using namespace boost::program_options;

const char HDRSEP='^';
const char HSEP='|';

const string VERSION="DX_slice v1.0 \n Copyright Ishanu Chattopadhyay 2019";
const string EMPTY_ARG_MESSAGE="Exiting. Type -h or --help for usage";


bool invalidChar (char c) 
{  
  return !( (unsigned int)c >32);   
} 
void stripUnicode(string & str) 
{ 
    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());  
}

int main(int argc, char* argv[])
{
  string DBfile="",ofile="out.dat";
  int AGEMAX=5,AGEMIN=0;

  options_description infor( "Program information");
  infor.add_options()
    ("help,h", "print help message.")
    ("version,V", "print version number");

  options_description usg( "Usage");
  usg.add_options()
    ("dbfile,D",value<string>(&DBfile), "database file []")
    ("agemax,A",value<int>(&AGEMAX), "Max Age [5]")
    ("agemin,a",value<int>(&AGEMIN), "Min Age [0]")  
    ("obfile,o",value<string>(&ofile), "output file [out.dat]");
 
  options_description desc( "\n\n\
                               ,%%%%%%%%%%%%%%%%%%%                            \n\
                         (%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                     \n\
                     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%,                \n\
                  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&             \n\
               *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&          \n\
             %%%%%%%%%%%%%%%%%%%%%%,          *%%%%%%%%%%%%%%%%%%%%%%%%        \n\
           %%%%%%%%%%%%%%%%%.                         /%%%%%%%%%%%%%%%%%%/     \n\
         %%%%%%%%%%%%%%%%                                 (%%%%%%%%%%%%%%%%    \n\
       %%%%%%%%%%%%%%%%                                      %%%%%%%%%%%%%%%%  \n\
     %%%%%%%%%%%%%%%%%                                         %%%%%%%%%%%%%%%.\n\
   %%%%%%%%%%%%%%%%%%                                                          \n\
%%%%%%%%%%%%%%%%%%%%%                 DX Parser                                \n\
   &%%%%%%%%%%%%%%%%%    copyright ishanu chattopadhyay 2015                   \n\
     &%%%%%%%%%%%%%%%%            ishanu@uchicago.edu          &%%%%%%%%%%%%%%#\n\
       %%%%%%%%%%%%%%%%                                      &%%%%%%%%%%%%%%%  \n\
         %%%%%%%%%%%%%%%%         PIPELINE SECTION 0       %%%%%%%%%%%%%%%%,   \n\
           %%%%%%%%%%%%%%%%%                           &%%%%%%%%%%%%%%%%%%     \n\
             %%%%%%%%%%%%%%%%%%%%%#            #%%%%%%%%%%%%%%%%%%%%%%%,       \n\
               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&          \n\
                  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%     %       \n\
                     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                \n\
                         %&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&                     \n\
                               %%%%%%%%%%%%%%%%%%%%,");
  desc.add(infor).add(usg);

  positional_options_description p;
  variables_map vm;
  if (argc == 1)
    {
      cout << EMPTY_ARG_MESSAGE << endl;
      return 1;
    }
  try
    {
      store(command_line_parser(argc, argv)
	    .options(desc)
	    .run(), vm);
      notify(vm);
    } 
  catch (std::exception &e)
    {
      cout << endl << e.what() 
	   << endl << desc << endl;
      return 1;
    }
  if (vm.count("help"))    
    {
      cout << desc << endl;
      return 1;
    }
  if (vm.count("version"))
    {
      cout << VERSION << endl; 
      return 1;
    }
  

  ofstream out(ofile.c_str());
  
  ifstream IN(DBfile.c_str());
  int NUM=UTIL__::FileSizeLines(IN);


  string line,line_="";
  int countl=0;
  while(getline(IN,line))
    {
      UTIL__::print_status(++countl/(0.0+NUM));
      stripUnicode(line);
      char c = line.back();
      if ((int)c == 92)
	{
	  line_+=line;
	  continue;
	}
      else
	{
	  line_+=line;
	}
      stringstream ss(line_);
      string hdr;
      if(getline(ss,hdr,HDRSEP))
	{
	  stringstream ssh(hdr);
	  string tok,id;
	  unsigned int count=0;
	  while(getline(ssh,tok,HSEP))
	    {
	      if(count==1)
		{
		  int age=atoi(tok.c_str());
		  if ((age<=AGEMAX) && (age>=AGEMIN) && (line_.back()!=HDRSEP))
			out << line_ << endl;
		  break;
		}
	      else
		id=tok;
	      count++;
	    }
	}
      line_="";
    }

  return 0;
}

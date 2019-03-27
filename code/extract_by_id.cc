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

const string VERSION="DX_extract_id v1.0 \n Copyright Ishanu Chattopadhyay 2019";
const string EMPTY_ARG_MESSAGE="Exiting. Type -h or --help for usage";


bool invalidChar (char c) 
{  
  return !( (unsigned int)c >32);   
} 
void stripUnicode(string & str) 
{ 
    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());  
}

bool checkid(string id,set<string>& valid_id)
{
  if (valid_id.find(id) != valid_id.end())
    {
      valid_id.erase(id);
      return true;
    }

  return false;
};


int main(int argc, char* argv[])
{
  string DBfile="",ofile="out.dat",IDfile="";
  int AGEMAX=5,AGEMIN=0;
  string PRINT_FIPS="";

  options_description infor( "Program information");
  infor.add_options()
    ("help,h", "print help message.")
    ("version,V", "print version number");

  options_description usg( "Usage");
  usg.add_options()
    ("dbfile,D",value<string>(&DBfile), "database file []")
    ("idfile,I",value<string>(&IDfile), "ID file []")
    ("agemax,A",value<int>(&AGEMAX), "Max Age [5]")
    ("agemin,a",value<int>(&AGEMIN), "Min Age [0]")  
    ("printfips,x",value<string>(&PRINT_FIPS), "print fips and gender [false if empty]")  
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
  
  char_separator<char> sep0(STRCOMMA);
  char_separator<char> sep(STRCARET);
  char_separator<char> sep_(STRPIPE);
  char_separator<char> sep__(STRCOLON);

  ofstream fout;
  if(PRINT_FIPS!="")
    fout.open(PRINT_FIPS.c_str());
  
  ifstream ID(IDfile.c_str());
  set<string> valid_id;
  string idline,thisid;
  while(getline(ID,idline))
    {
      stringstream ss(idline);
      if(ss>>thisid)
	valid_id.insert(thisid);
    }
  ID.close();
  
  ofstream out(ofile.c_str());
  
  ifstream IN(DBfile.c_str());
  int NUM=UTIL__::FileSizeLines(IN);


  string line,line_="";
  int countl=0;
  while(getline(IN,line))
    {
      if(valid_id.empty())
	return 0;
      
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

	  vector <string> V_;
	  set<string> _fips_;
	  tokenizer<char_separator<char>> tokens_(hdr,sep_);
	  for (const auto& t : tokens_) 
	    V_.push_back(trim(t));

	  string id_=V_[0];
	  unsigned int age_=atoi(V_[1].c_str());
	  string gender_=V_[3];
	  string fips_=V_[4];
	  unsigned int yr_=atoi(V_[5].c_str());

	  _fips_.clear();
	  tokenizer<char_separator<char>>
	    fips_tokens_(fips_,sep0);
	  for (const auto& t : fips_tokens_)
	    _fips_.insert(t);

	  if ((age_<=AGEMAX)
	      && (age_>=AGEMIN)
	      && checkid(id_,valid_id)
	      && (line_.back()!=HDRSEP))
	    {
	      out << line_ << endl;
	      if(PRINT_FIPS!="")
		{
		  fout << id_ << ","
		       << gender_ << ",";
		  for(auto itr=_fips_.begin();
		      itr!=_fips_.end();
		      ++itr)
		    fout << *itr << " ";
		  fout << endl;
		}
	    }	  
	}
      line_="";
    }

  if(PRINT_FIPS!="")
    fout.close();
  
  return 0;
}


//check fips print code for endl error

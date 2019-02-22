#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <exception>
#include <boost/timer/timer.hpp>
#include <sys/resource.h>
#include <boost/filesystem.hpp>

#include "procx.h"

using namespace boost::program_options;

const string VERSION="DBPROC_gen v0.9314 \n Copyright Ishanu Chattopadhyay 2019";
const string EMPTY_ARG_MESSAGE="Exiting. Type -h or --help for usage";
const string USAGE="-P phnfile  -D DXfile -w DATA_SEQ_DIR   -a 5 -T 1 -F ALLFIPS.dat -z 1";

  
bool TIMER=false;

//---------------------------------
//---------------------------------
//---------------------------------
int main(int argc, char* argv[])
{
  string phnfile="phn.dat", dxfile="";
  string fipsfile="", resultdirectory="./", datadirectory="./";

  int AGE_MAX=5;
  set <string> FILES_;
  vector <string> vFILES_;
  set <unsigned int> TRNCT_, EXCLD_,NECESS_;
  vector <unsigned int> vTRNCT_, vEXCLD_,vNECESS_;
  bool zeropref=true;
  string intfile="integrated.log";

  options_description infor( "Program information");
  infor.add_options()
    ("help,h", "print help message.")
    ("version,V", "print version number");


  options_description usg( "Usage");
  usg.add_options()
    ("phnfile,P",value<string>(), "phenotype categoroes spec file []")
    ("fipsfile,F",value<string>(), "FIPS spec file")
    ("intfile,I",value<string>(&intfile), "Integrated file")
    ("fips,f",value<vector<string> >()->multitoken(), "FIPS spec in command line")
    ("dxfile,D",value< string>(), "dx file []");
  
  options_description outputopt( "Output options");
  outputopt.add_options()
    ("resultdirectory,w",value< string>(), "directory for string output files [./]")
    ("agemax,a",value<int>(), "max age [5]")
    ("zeropref,z",value<bool>(), "zero pref. True/on/1: birth synced [1]")
    ("truncate,T",value<vector<unsigned int> >()->multitoken(), "truncate at first occurrence []")
    ("exclude,X",value<vector<unsigned int> >()->multitoken(), "exclude any occurrence []")
    ("necess,N",value<vector<unsigned int> >()->multitoken(), "must occur []")
    ("timer,t",value<bool>(), "show timer [true]");
  
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
  desc.add(infor).add(usg).add(outputopt);

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
  
  if (vm.count("timer"))
    TIMER=vm["timer"].as<bool>();
  if (vm.count("phnfile"))
    phnfile=vm["phnfile"].as<string>();
  if (vm.count("fipsfile"))
    fipsfile=vm["fipsfile"].as<string>();
  if (vm.count("dxfile"))
    dxfile=vm["dxfile"].as<string>();
  if (vm.count("fips"))
    vFILES_=vm["fips"].as<vector<string> >();
  if (vm.count("resultdirectory"))
    resultdirectory=vm["resultdirectory"].as<string>();
  if (vm.count("agemax"))
    AGE_MAX=vm["agemax"].as<int>();
  if (vm.count("truncate"))
    vTRNCT_=vm["truncate"].as< vector<unsigned int> >();
  if (vm.count("exclude"))
    vEXCLD_=vm["exclude"].as< vector<unsigned int> >();
  if (vm.count("necess"))
    vNECESS_=vm["necess"].as< vector<unsigned int> >();
  if (vm.count("zeropref"))
    zeropref=vm["zeropref"].as<bool>();
  
  for(unsigned int i=0;i<vFILES_.size();++i)
    FILES_.insert(vFILES_[i]);
  for(unsigned int i=0;i<vTRNCT_.size();++i)
    TRNCT_.insert(vTRNCT_[i]);
  for(unsigned int i=0;i<vEXCLD_.size();++i)
    EXCLD_.insert(vEXCLD_[i]);
  for(unsigned int i=0;i<vNECESS_.size();++i)
    NECESS_.insert(vNECESS_[i]);

  if(fipsfile!="")
    {
      string linef;
      ifstream IF(fipsfile.c_str());
      while(getline(IF,linef))
	FILES_.insert(linef);
      IF.close();
    }

  if (!boost::filesystem::exists(dxfile))
    MESSAGE("ERROR: DB FILE MISSING");
  if (!boost::filesystem::exists(phnfile))
    MESSAGE("ERROR: PHN FILE MISSING");

  if (!boost::filesystem::exists(resultdirectory))
    boost::filesystem::create_directory(resultdirectory);

  _phenotype_ PHN(phnfile,resultdirectory+"/"+intfile);
  PHN.setup(TRNCT_,EXCLD_,NECESS_);
  PHN.set_pref(zeropref);
  
  PHN.print_setup();
  
  ifstream IN(dxfile.c_str());
  PHN.set_filelines(UTIL__::FileSizeLines(IN));
  PHN.set_agemax(AGE_MAX);
  
  set<string> TYPE_={"POS","NEG"};
  set<string> GENDER_={"M","F"};

  cout  << "parsing... " << endl;

  string subject;
  while(getline(IN,subject))
    UTIL__::print_status(PHN<<subject);
  cout << endl;


    
  PHN.write_log(resultdirectory);
  IN.close();

  return 0;
}
//---------------------------------
  

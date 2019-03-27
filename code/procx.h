#ifndef PROCCODES_HEADERFILE_H
#define PROCCODES_HEADERFILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <set>
#include <functional>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <memory>
#include <regex>
//#include <boost/algorithm/string.hpp>

#define MESSAGE(x) UTIL__::_MESSAGE((x), __PRETTY_FUNCTION__)

using namespace std;
using namespace boost;

//---------------------------------
extern const char* STRPIPE;
extern const char* STRCOLON;
extern const char* STRCARET;
extern const string HEALTHY_;
extern const char* STRCOMMA;

/*! Print utility */
ostream& operator << (ostream &out, vector <unsigned int> &s);\

/*! Print utility */
ostream& operator << (ostream &out, vector <string> &s);
ostream& operator << (ostream &out, set <string> &s);
ostream& operator << (ostream &out, set <unsigned int> &s);

//---------------------------------

/*!
  Utility function to remove leading and trailing whitespace 
  substring \c consist \c string \b whitespace   from a given \c string.
  The white space can be given to be any string, default is \b whitespace
*/
string trim(const string& str,
	    const string& whitespace = " ");

//---------------------------------------------
/*!
Basic subject class. Reads DX file, and makes sure
memory efficient conversion to symbol streams
 */

class _subject_
{
private:
  
  string _id_;
  unsigned int _age0_;
  string _gender_;
  set <string> _fips_;
  unsigned int _yr0_;

  vector <string> _code_;
  vector <set<string> > _code__;
  map<int,set <pair<int,string> > >  wk_age_code_;
  map<int,set <string> > wk_code_;
  
public:

  _subject_(string str, 
	    int age=-1,
	    string gender=""); //these are rejection constraints

  bool empty();
  unsigned int age0();
  string& gender();
  set<string>& fips();
  string yr0();
  string& id();
  string fips_state();

  vector <unsigned int> week();
  vector <string> code();

  // void print();

  // vector<string> &operator() (void);
  vector<set<string> >&codeseq(bool FIRST_NON_TRIVIAL_SYM=true);

};

//---------------------------------------------

/*!
  Phenotype mapper class. 
  Maps phenotype names to alphabet symbols.
 */

class _phenotype_
{
  /*! maps phn names (icd9 codes) to name of 
    the category : infections, autism etc   */
  map<string,string> _phn_cat_;

  /*! maps category name to alphabet symbol */
  map<string,unsigned int> _cat_sym_;

  /*! Number of distinct categories + other. Note Healthy is zero,
   other is alphabet-1*/
  unsigned int alphabet;

  vector <unsigned int> _phv_;

  map<unsigned int,unsigned int> _remap_;

  set <unsigned int> _TRNCT_;
  set <unsigned int> _EXCLD_;
  set <unsigned int> _NCSRY_;

  map<string,vector<vector<unsigned int> > > LOG_; //missing patient id
  map<string,vector<string> > LOGid_;
  
  int _lines_proc_;
  int _FSZ_;
  int _agemax_;
  
  set<unsigned int> ss;
  bool _FIRST_NON_TRIVIAL_SYM_;

  string intlog;
  ofstream Dout;
  
public:

  _phenotype_(string filename,string singlefile="intlog.dat");
  _phenotype_();
 
  void add(string,string);

  unsigned int& operator[] (const string);
  set <unsigned int>& operator[] (const set<string>&);
  /*
  bool operator() (const vector<string>&, 
		   std::ostream &out = std::cout,
		   std::ostream &Tout = std::cout);
  */
  bool operator() (const vector<set<string> >&, 
		   string out,
		   string Tout,string id="");

  unsigned int remap(unsigned int);

  void setup(set <unsigned int> _TRNCT={},
	     set <unsigned int> _EXCLD={},
	     set <unsigned int> _NCSRY={});

  void set_agemax(int AGEMAX=-1);
  void set_filelines(int);
  
  double operator<< (string subject);
  
  void clear_setup();

  void write_log(string directory="./");
  void print_setup();

  void set_pref(bool);

};
//---------------------------------
//---------------------------------
/* @bug
   Are all relevant data in file in asingle lines?
   When the number of lines requested is small,
   we need to make random starts in the file, and not always 
   start at beginning of file.
 */



/**
\b class _selection_ is designed for random selection of lines
from file specified by string. The number of lines selected 
is given by \b count. When \b count is 0, we select 1 line per file.
 */

class _selection_
{
  string _filepath_;
  int _count_;
  set<string> _ss_;
  
 public:
  _selection_(string,int count=-1);

  void get(string pathtof,set<string>&);

};

//---------------------------------


//---------------------------------
namespace UTIL__
{
  unsigned int FileRead( istream & is, vector <char> & buff );
  unsigned int CountLines( const vector <char> & buff, int sz );
  int FileSizeLines(ifstream& IN);
  void print_status(float progress);

  /*!                                                                           
    Utility function to print message. If the string ERROR occurs in the message                                   
    then execution is aborted                                                   
   */
  void _MESSAGE(string,
                const char *caller);

  
//---------------------------------------------

};

//---------------------------------




#endif

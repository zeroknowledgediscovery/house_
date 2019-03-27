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

using namespace std;
using namespace boost;

//---------------------------------
typedef string pid__;
typedef string code__;
typedef map<pid__,string> record__;
typedef map<code__,double> codecount__;
typedef vector<code__> hist__;
//---------------------------------------------

const char* STRPIPE = "|";
const char* STRCOMMA=",";
const char* STRCOLON=":";
const char* STRCARET="^";
const string STRBRK="\\";
const string STRCOLON_=":";
//---------------------------------

int LB_=0;
int UB_=50;

/*!
  Utility function to remove leading and trailing whitespace 
  substring \c consist \c string \b whitespace   from a given \c string.
  The white space can be given to be any string, default is \b whitespace
*/
string trim(const string& str,
	    const string& whitespace = " ");

//---------------------------------
//---------------------------------------------
string trim(const string& str,
	    const string& whitespace)
{
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
};
//---------------------------------------------
/*
string removeSubstrs__(string& s) {
  s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );
  return s;
};
*/
//---------------------------------------------
string removeSubstrs__(string& s,
                   const string& p) {
  size_t n = p.length();
  bool FOUND=false;
  for (size_t i = s.find(p);
       i != string::npos;
       i = s.find(p))
    s.erase(i, n);
  return s;
};
//---------------------------------------------
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace)
{
  size_t pos = 0;
  while((pos = subject.find(search, pos))!= std::string::npos)
    {
      subject.replace(pos, search.length(), replace);
      pos += replace.length();
    }
}
//---------------------------------------------
//---------------------------------------------
namespace UTIL__
{
  void get(map<int,set<code__> > M,
	   pair<int,int> wk_bnd,
	   codecount__& C);
  string coarsen(string S, const string s=".");
  void exception(string message,bool ext=false);
  unsigned int FileRead( istream & is, vector <char> & buff );
  unsigned int CountLines( const vector <char> & buff, int sz );
  int FileSizeLines(ifstream& IN);
  void print_status(float progress);
  void normalize(codecount__& C);
//---------------------------------------------
}
//---------------------------------------------
void UTIL__::normalize(codecount__& C)
{
  double s=0.0;
  for(codecount__::iterator itr=C.begin();
      itr!=C.end();
      ++itr)
    s+=itr->second;
  if(s>0)
    for(codecount__::iterator itr=C.begin();
	itr!=C.end();
	++itr)
      itr->second/=s;
  return;
};

//---------------------------------------------
string UTIL__::coarsen(string S, const string s)
{
  if(S.find(s)==string::npos)
    return S;
  return S.substr(0,S.find(s));
};


//---------------------------------------------
void UTIL__::get(map<int,set<code__> > M,
	   pair<int,int> wk_bnd,
	   codecount__& C)
{
  map<int,set<code__> >::iterator itr1=M.lower_bound(wk_bnd.first);
  map<int,set<code__> >::iterator itr2=M.upper_bound(wk_bnd.second);

  bool HEALTHY=true;
 
  for(map<int,set<code__> >::iterator itr=itr1;itr!=itr2;
      ++itr)
    {
      HEALTHY=false;
      for(set<code__>::iterator itr_=itr->second.begin();
	  itr_!=itr->second.end();
	  ++itr_)
	if(trim(*itr_).length()>1)
	  C[UTIL__::coarsen(trim(*itr_))]++;
    }

  if(HEALTHY)
    C["00H"]++;
  
  return;
};

//---------------------------------------------
void UTIL__::exception(string message,bool ext)
{
  cout << message <<endl;
  if(ext)
    exit(0);
};
//---------------------------------------------
//---------------------------------
unsigned int UTIL__::FileRead( istream & is, vector <char> & buff )
{
    is.read( &buff[0], buff.size() );
    return is.gcount();
}
//---------------------------------
unsigned int UTIL__::CountLines( const vector <char> & buff, int sz )
{
    int newlines = 0;
    const char * p = &buff[0];
    for ( int i = 0; i < sz; i++ ) {
    	if ( p[i] == '\n' ) {
    		newlines++;
    	}
    }
    return newlines;
}
//---------------------------------
int UTIL__::FileSizeLines(ifstream& IN)
{
  int FSZ = 0;
  std::vector <char> buff( 1024*1024 );
  while( int cc = FileRead( IN, buff ) )
    FSZ += CountLines( buff, cc );
  IN.clear();
  IN.seekg(0, ios::beg);
  
  return FSZ;
};
//---------------------------------
void UTIL__::print_status(float progress)
{
  string str="";
  char buff[1024]; 
  sprintf(buff,"%c[%d;%d;%dm",27,0,32,40);
  str+=buff;
  string str1=""; 
  sprintf(buff,"%c[%dm",27,0);
  str1+=buff;


  int barWidth = 40;

  std::cout << str<< "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos) std::cout << "=";
    else if (i == pos) std::cout << ">";
    else std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r" << str1;

  std::cout.flush();
}

//---------------------------------------------
//---------------------------------------------
class _emrdb_
{
  record__ _records_;

  codecount__  C;
  
  unsigned int FSZ_;
  map<string,map<int,set <string> > > _hist_;

  map<code__,string> _code_desc_;
  map<string,double> _agg_;
  
public:

  _emrdb_(string filename);
  void print_count(ostream& OUT);
  double get(hist__);
  codecount__& get();
  void set_desc(string filename);

  map<string,double>& aggregate(bool normalize=true);
  
};
//---------------------------------------------

map<string,double>& _emrdb_::aggregate(bool normalize)
{
  for(codecount__::iterator itr=C.begin();
      itr!=C.end();
      ++itr)
    if(_code_desc_.find(itr->first)!=_code_desc_.end())
      _agg_[_code_desc_[itr->first]]+=itr->second;

  if(normalize)
    {
      double s=0.0;
      for(map<string,double>::iterator itr=_agg_.begin();
	  itr!=_agg_.end();
	  ++itr)
	s+=itr->second;
      if(s>0.0)
	for(map<string,double>::iterator itr=_agg_.begin();
	    itr!=_agg_.end();
	    ++itr)
	  itr->second/=s;
    }
  
  return _agg_;
};

//---------------------------------------------

void _emrdb_::set_desc(string filename)
{
  string line;
  code__ cod;
  ifstream IN(filename.c_str());
  while(getline(IN,line))
    {
      stringstream ss(line);
      if(ss>>cod)
	{
	  string tmp;
	  getline(ss,tmp);
	  _code_desc_[cod]=tmp;
	}
    }
  IN.close();
};

//---------------------------------------------
codecount__& _emrdb_::get(void)
{
  return C;
};

//---------------------------------------------
double _emrdb_::get(hist__ s)
{
  if(s.size()==1)
    if(C.find(s[0])!=C.end())
      return C[s[0]];
  return 0.0;
};

//---------------------------------------------
void _emrdb_::print_count(ostream& OUT)
{
  for(codecount__::iterator itr=C.begin();
      itr!=C.end();
      ++itr)
    OUT << "code " << itr->first
	<< " : " << itr->second
	<< endl;
  return;
};
//---------------------------------------------
_emrdb_::_emrdb_(string filename)
{
  char_separator<char> sep0(STRCOMMA);
  char_separator<char> sep(STRCARET);
  char_separator<char> sep_(STRPIPE);
  char_separator<char> sep__(STRCOLON);

  
  ifstream IN(filename.c_str());

  FSZ_=UTIL__::FileSizeLines(IN);
  

  string lastline="",thisline="";
  unsigned int line_proc_=0;

  //  codecount__ C;

  pair<int,int> wk_bnd;
  wk_bnd=make_pair(LB_,UB_);
  
  
  while(getline(IN,thisline))
    {
      if(trim(thisline).length()<5)
	continue;
      line_proc_++;
      thisline=trim(thisline);
      thisline=removeSubstrs__(thisline,STRBRK);

      /*
      if(thisline.substr(thisline.length()-1,1)==STRBRK)
	{
	  lastline=removeSubstrs__(thisline,STRBRK);
	  continue;
	}
      if(lastline!="")
	{
	  if(thisline.substr(0,1)==STRBRK)
	    thisline=lastline+removeSubstrs__(thisline,STRBRK);
	  else
	    UTIL__::exception("ERROR: \\ mismatch",true);
	}
      */
      vector<string> V;
      tokenizer<char_separator<char>> tokens(thisline,sep);
      for (const auto& t : tokens) 
	V.push_back(trim(t));
      
      string hdr=V[0];

      //processing hdr
      vector <string> V_;
      tokenizer<char_separator<char>> tokens_(hdr,sep_);
      for (const auto& t : tokens_) 
	V_.push_back(trim(t));

      string id_=V_[0];
      unsigned int age_init_=atoi(V_[1].c_str());
      string gender_=V_[3];
      string fips_=V_[4];
      unsigned int yr_init_=atoi(V_[5].c_str());

      id_+= STRCOLON_+gender_
	+STRCOLON_+fips_;
      
      set <string> _fips_;
      map<int,set <pair<int,string> > >  wk_age_code_;
      map<int,set <string> > wk_code_;
     
      _fips_.clear();
      tokenizer<char_separator<char>> fips_tokens_(fips_,sep0);
      for (const auto& t : fips_tokens_)
	_fips_.insert(t);

      //processing data after hdr
      tokenizer<char_separator<char>> tokens__(V[1],sep_);
      for (const auto& t : tokens__) 
	{
	  vector <string> V__;
	  tokenizer<char_separator<char>> tokens___(t,sep__);

	  unsigned int count=0;
	  string code_;
	  int week_;
	  int age_;
	  for (const auto& t_ : tokens___) 
	    {
	      if(count==0)
		code_=trim(t_);
	      if(count==1)
		age_=atoi(t_.c_str());
	      if(count==2)
		week_=atoi(t_.c_str());
	      count++;
	    }
	  wk_age_code_[week_].insert(make_pair(age_,code_));
	}

      int week_firstrecord_= wk_age_code_.begin()->first;
      int age_firstrecord_=
	(wk_age_code_.begin()->second.begin()->first)*52+26;

      int del_=age_firstrecord_-week_firstrecord_;
      
      wk_code_.clear();
      for(map<int,set <pair<int,string> > >::iterator itr
	    =wk_age_code_.begin();
	  itr!=wk_age_code_.end();
	  ++itr)
	for(set <pair<int,string> >::iterator itr_
	      =itr->second.begin();
	    itr_!=itr->second.end();
	    ++itr_)
	  wk_code_[itr->first+del_].insert(itr_->second);

      UTIL__::get(wk_code_,wk_bnd,C);
      UTIL__::normalize(C);
      
      //_hist_[id_]=wk_code_;
      
      UTIL__::print_status((line_proc_+0.0)/(FSZ_+0.0));

    }
};
//---------------------------------------------
//---------------------------------------------

namespace HOUSE__
{
  double ZERO=0.00000000001;
  double distance(hist__ h1,hist__ h2,_emrdb_&);
  double jaccard(hist__ h1,hist__ h2);
  double uprob(hist__ h,_emrdb_&);
}
//---------------------------------------------
double HOUSE__::distance(hist__ h1,hist__ h2,_emrdb_& E)
{
  //  return (1- exp(-0.5*((1.0/(uprob(h1,E)))+(1.0/(uprob(h2,E)))-2.0)))
  //  *jaccard(h1,h2);
  return (0.5*((1.0/(uprob(h1,E)))+(1.0/(uprob(h2,E)))-2.0))
    *jaccard(h1,h2);
};

//---------------------------------------------
double HOUSE__::uprob(hist__ h,_emrdb_& E)
{
  if(h.empty())
    return 1.0;
  if(h.size()==1)
    return E.get(h)>0?E.get(h):ZERO;
};

//---------------------------------------------
double HOUSE__::jaccard(hist__ h1,hist__ h2)
{
  if(h1==h2)
    return 0.0;
  if(h1.size()*h2.size()==1)
    return 1.0;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

int main(int argc, char* argv[])
{
  string filename="",desfile="icd9data2.dat";
  string outfile="out.txt";
  if(argc>1)
    filename=argv[1];

  if(argc>2)
    LB_=atoi(argv[2]);
  if(argc>3)
    UB_=atoi(argv[3]);
  if(argc>4)
    outfile=argv[4];
  
  _emrdb_ E(filename);

  cout << endl;
  //E.print_count(cout);

  E.set_desc(desfile);
  map<string,double> H=E.aggregate();

  ofstream oag("agg.txt");
  for(map<string,double>::iterator itr=H.begin();
      itr!=H.end();
      ++itr)
    oag << "\"" << trim(itr->first) << "\"," << itr->second << endl;
  oag.close();

  ofstream out(outfile.c_str());
  E.print_count(out);
  out.close();

  codecount__ C=E.get();
  ofstream outM((outfile+"mat").c_str());
  for(codecount__::iterator itr=C.begin();
      itr!=C.end();
      ++itr)
    {
      for(codecount__::iterator itr_=C.begin();
	  itr_!=C.end();
	  ++itr_)
	outM << HOUSE__::distance(vector<code__> (1,itr->first),
				  vector<code__> (1,itr_->first),E) << " ";
      outM << endl;
    }
  outM.close();

  return 0;
};
  












//---------------------------------------------
//---------------------------------------------



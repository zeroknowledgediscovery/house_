#include "procx.h"
#define INDIVIDUAL_FILES 0
//static const char* STRCOMMA = ",";
const char* STRPIPE = "|";
const char* STRCOMMA=",";
const char* STRCOLON=":";
const char* STRCARET="^";
const string HEALTHY_="H";

//------------------------------------                                          
void UTIL__::_MESSAGE(string m,const char *caller)
{

  cout << m
       << " in function "
       << std::string(caller) << endl;
  if(m.find("ERROR") != std::string::npos)
    exit(0);

};


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


ostream& operator << (ostream &out, set <unsigned int> &s)
{
  for (set<unsigned int>::iterator itr=s.begin();
       itr!=s.end();
       ++itr)
    out << *itr << " ";
  if(!s.empty())
    out << endl;
  return out;
}
//---------------------------------

ostream& operator << (ostream &out, set <string> &s)
{
  for (set<string>::iterator itr=s.begin();
       itr!=s.end();
       ++itr)
    out << *itr << endl;
  /*  if(!s.empty())
      out << endl;*/
  return out;
}
//---------------------------------

ostream& operator << (ostream &out, vector <string> &s)
{
  for (unsigned int i=0; i<s.size(); i++)
    out << s[i] << " ";
  if(!s.empty())
    out << endl;
  return out;
}//---------------------------------
//---------------------------------
ostream& operator << (ostream &out, vector <unsigned int> &s)
{
  for (unsigned int i=0; i<s.size(); i++)
    out << s[i] << " ";
  if(!s.empty())
    out << endl;
  return out;
}//---------------------------------


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

vector<set<string> >&_subject_::codeseq(bool FIRST_NON_TRIVIAL_SYM)
{  
  if(wk_code_.empty())
    return _code__;

  unsigned int beg_=0;
  unsigned int end_=wk_code_.rbegin()->first;

  _code__.clear();
  
  for(unsigned int i=beg_;i<=end_;++i)
    if(wk_code_.find((int)i)!=wk_code_.end())
      {
	_code__.push_back(wk_code_[(int)i]);
	FIRST_NON_TRIVIAL_SYM=true;
      }
    else
      if(FIRST_NON_TRIVIAL_SYM)
	_code__.push_back(set <string> {HEALTHY_});
  
  return _code__;
};
//---------------------------------
string& _subject_::gender()
{
  return _gender_;
};
//---------------------------------
string& _subject_::id()
{
  return _id_;
};
//---------------------------------
set<string>& _subject_::fips()
{
  return _fips_;
};
//---------------------------------
_subject_::_subject_(string str, 
		     int age__,
		     string gender__)
{
  _id_="000";
  char_separator<char> sep0(STRCOMMA);
  char_separator<char> sep(STRCARET);
  char_separator<char> sep_(STRPIPE);
  char_separator<char> sep__(STRCOLON);
  vector <string> V;

  tokenizer<char_separator<char>> tokens(str,sep);
  for (const auto& t : tokens) 
    V.push_back(trim(t));

  string hdr=V[0];

  vector <string> V_;
  tokenizer<char_separator<char>> tokens_(hdr,sep_);
  for (const auto& t : tokens_) 
    V_.push_back(trim(t));

  string id_=V_[0];
  unsigned int age_=atoi(V_[1].c_str());
  string gender_=V_[3];
  string fips_=V_[4];
  unsigned int yr_=atoi(V_[5].c_str());

  _fips_.clear();
  tokenizer<char_separator<char>> fips_tokens_(fips_,sep0);
  for (const auto& t : fips_tokens_)
    _fips_.insert(t);
  
  bool GO_=false;
  if((gender__=="") || (gender__==gender_))
    if((age__==-1) || ((int)age_<=age__))
      GO_=true;

  if(GO_)
    {

      cout << id_ << " " << str << endl;
      
      _id_=id_;
      _age0_=age_;
      _gender_=gender_;
      _yr0_=yr_;

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
    }
};

//---------------------------------
//---------------------------------
//---------------------------------

_phenotype_::_phenotype_(string filename,string singlefile)
{
  _lines_proc_=0;
  _agemax_=-1;
  _FSZ_=-1;
  
  string line;
  ifstream IN(filename.c_str());
  alphabet=1;

  while(getline(IN,line))
    {
      stringstream ss(line);
      string name_,phn__;

      if(ss>>name_)   //name_ is name of category (infections, autism etc. //
	while(ss>>phn__)  // We use "influenza" in the phnfile //
	  _phn_cat_[phn__]=name_;

      if(_cat_sym_.find(name_)==_cat_sym_.end())
	_cat_sym_[name_] = alphabet++;
    }

  _cat_sym_[HEALTHY_]=0;
  _cat_sym_["other"]=alphabet++;

  intlog=singlefile;

  if(!INDIVIDUAL_FILES)
    Dout.open(intlog.c_str());
  
};
//---------------------------------

unsigned int& _phenotype_::operator[] (const string phn__)
{
  if (phn__==HEALTHY_)
    return _cat_sym_[HEALTHY_];

  if(_phn_cat_.find(phn__)!=_phn_cat_.end())
    if(_cat_sym_.find(_phn_cat_[phn__])!=_cat_sym_.end())
      return _cat_sym_[_phn_cat_[phn__]];

  return _cat_sym_["other"];
};
//---------------------------------
set<unsigned int>& _phenotype_::operator[] (const set<string>& phn__)
{
  ss.clear();
  for(set<string>::const_iterator itr=phn__.begin();
      itr!=phn__.end();
      ++itr)
    ss.insert((*this)[*itr]);
  return ss;
};
//---------------------------------

bool _phenotype_::operator() (const vector<set<string> >& phn__,
			      string out,
			      string Tout,
			      string id)
{
  if(id=="000")
    return false;
  
  bool TRUNCATE_=false;
  set<unsigned int> _NCSRY(_NCSRY_);
  _phv_.clear();
  
  for(unsigned int i=0;i<phn__.size();++i)
    {
      set<string> tmps=phn__[i];
      set<unsigned int>  this_set_=(*this)[phn__[i]];

      unsigned int  NEC_FOUND_=10000000;
      unsigned int this_=0;
      
      for(set<unsigned int>::iterator itr
	    =this_set_.begin();
	  itr!=this_set_.end();
	  ++itr)
	{
	  //cout << this_ << endl;
	  this_=*itr;
	  
	  if(_EXCLD_.find(this_)!=_EXCLD_.end())
	    return false;

	  if(!_NCSRY.empty())
	    {
	      _NCSRY.erase(this_);
	      NEC_FOUND_=this_;
	    }

	  if(_TRNCT_.find(this_)!=_TRNCT_.end())
	    TRUNCATE_=true;
	}

      if(_NCSRY_.find(NEC_FOUND_)!=_NCSRY_.end())
	this_=NEC_FOUND_;

      if(TRUNCATE_)
	{
	  if(_NCSRY.empty())
	    {
	      if(INDIVIDUAL_FILES)
		{
		  LOG_[Tout].push_back(_phv_);
		  if(id!="")
		    LOGid_[Tout].push_back(id);
		}
	      else
		{
		  Dout  << Tout << " " << id << " " << _phv_;
		}
	      return true;
	    }
	  return false;
	}
      else
	_phv_.push_back(this->remap(this_)); 
    }

  if(!_NCSRY.empty())
    return false;

  if(INDIVIDUAL_FILES)
    {
      LOG_[out].push_back(_phv_);
      if(id!="")
	LOGid_[out].push_back(id);
    }
  else
    {
      Dout  << out << " " << id << " " << _phv_;
    }

  
  return true;
};

//---------------------------------
unsigned int _phenotype_::remap(unsigned int s)
{
  if(_remap_.find(s)!=_remap_.end())
    return _remap_[s];
  return s;
};
//---------------------------------
void _phenotype_::setup(set <unsigned int> _TRNCT,
			set <unsigned int> _EXCLD,
			set <unsigned int> _NCSRY)
{
  _TRNCT_=_TRNCT;
  _EXCLD_=_EXCLD;
  _NCSRY_=_NCSRY;

  unsigned int num=1;
  for(map<string,unsigned int>::iterator itr
	=_cat_sym_.begin();
      itr!=_cat_sym_.end();
      ++itr)
    {
      if((_TRNCT_.find(itr->second)==_TRNCT_.end())
	 &&(_EXCLD_.find(itr->second)==_EXCLD_.end())
	 &&(itr->first!=HEALTHY_)
	 &&(itr->first!="other"))
	_remap_[itr->second] = num++;
    }
  _remap_[_cat_sym_["other"]] = num;
  _remap_[_cat_sym_[HEALTHY_]] = 0;
  
  for(map<string,unsigned int>::iterator itr
	=_cat_sym_.begin();
      itr!=_cat_sym_.end();
      ++itr)
    if(_TRNCT_.find(itr->second)!=_TRNCT_.end())
      _remap_[itr->second]=num+1;
  
};
//---------------------------------
void _phenotype_::print_setup()
{
  for (const auto& itr : _phn_cat_)
    cout << itr.first << " : "
	 << itr.second << endl;
  for (const auto& itr : _cat_sym_)
    cout << itr.first << " | "
	 << itr.second << endl;
  for (const auto& itr : _remap_)
    cout << itr.first << " - "
	 << itr.second << endl;

};
//---------------------------------
void _phenotype_::clear_setup()
{
  _TRNCT_.clear();
  _EXCLD_.clear();
  _NCSRY_.clear();
};
//---------------------------------
double _phenotype_::operator<< (string subject)
{  
  _subject_ sub(subject,_agemax_);
  set<string> fips_=sub.fips();

  for(set<string>::iterator itr=fips_.begin();
      itr!=fips_.end();
      ++itr)
    (*this)(sub.codeseq(_FIRST_NON_TRIVIAL_SYM_),
	    sub.gender()+"NEG"+*itr,
	    sub.gender()+"POS"+*itr,
	    sub.id());
  if(_FSZ_>0)
    return (++_lines_proc_)/(_FSZ_+0.0);
  else
    return ++_lines_proc_;
};
//---------------------------------
void _phenotype_::set_pref(bool bb)
{
  _FIRST_NON_TRIVIAL_SYM_=bb;
}
//---------------------------------
void _phenotype_::set_agemax(int age__)
{
  _agemax_=age__;
};
//---------------------------------
void _phenotype_::set_filelines(int LN__)
{
  _FSZ_=LN__;
};
//---------------------------------
void _phenotype_::write_log(string directory)
{
  int count=0;
  cout << "writing files.." << endl;
  for(map<string,vector<vector<unsigned int> > >::iterator itr
	=LOG_.begin();
      itr!=LOG_.end();
      ++itr)
    {
      ofstream OUT((directory+"/"+itr->first).c_str());
      for(vector<vector<unsigned int> >::iterator itr_
	    =itr->second.begin();
	  itr_!=itr->second.end();
	  ++itr_)
	OUT << *itr_;
      OUT.close();
      UTIL__::print_status(((count++)+1)/(LOG_.size()+0.0));
    }
  cout << endl;

  ofstream OUT_((directory+"/"+"id.log").c_str());
  for(map<string,vector<string> >::iterator itr
	=LOGid_.begin();
      itr!=LOGid_.end();
      ++itr)
    OUT_<< itr->first << " : " << itr->second;
  OUT_.close();
};
//---------------------------------
//---------------------------------
//---------------------------------
_selection_::_selection_(string path_,int count)
{
  _filepath_=path_;
  _count_=count;
  srand (time(NULL));
};

//---------------------------------
void _selection_::get(string fl,set<string>& ss)
{
  string thisfile=_filepath_+fl;
  
  string line;
  ifstream IN(thisfile);
  int count=0;
  if(_count_==-1)
    {
      while(getline(IN,line))
	if((rand()%2)&& (trim(line)!=""))
	  ss.insert(trim(line));
    }
  else
    {
      while(getline(IN,line) 
	    && count<=_count_)
	if((rand()%2) && (trim(line)!=""))
	  {
	    ss.insert(trim(line));
	    cout << fl << endl;
	    count++;
	  }
    }
  IN.close();
  return ;
};
//---------------------------------

/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: DistanceToBottom.cpp                                        */
/*    DATE:                                                 */
/************************************************************/
using namespace std;
#include <iterator>
#include <cmath>
#include <random>
#include "MBUtils.h"
#include "GeomUtils.h"
#include "DistanceToBottom.h"

double rand_gen() {
   // return a uniformly distributed random value
   return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}
double normalRandom() {
   // return a normally distributed random value
   double v1=rand_gen();
   double v2=rand_gen();
   return cos(2*3.14*v2)*sqrt(-2.*log(v1));
}

//---------------------------------------------------------
// Constructor

DistanceToBottom::DistanceToBottom()
{
	m_isX = false;
	m_isY = false;
	m_IsHeading = false;
	m_isGlebokoscZanurzenia = false;
	m_glebokoscPodstawowa = 50;
	m_run = false;
	m_szerokoscWiazkiSonaruPionowa = 1;
	m_szerokoscWiazkiSonaruPozioma = 2;
	m_zasiegSonaru = 30;
	
	m_sigma = 1;
    m_mi = 0;
}

//---------------------------------------------------------
// Destructor

DistanceToBottom::~DistanceToBottom()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool DistanceToBottom::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
	
	string key   = msg.GetKey();
	
	if(key == "NAV_X" && msg.IsDouble())
		{
		m_currentPosition.set_vx(msg.GetDouble());
		m_isX = true;
		}
	 else  if(key == "NAV_Y" && msg.IsDouble())
		{
		m_currentPosition.set_vy(msg.GetDouble());
		m_isY = true;
		}
	else  if(key == "NAV_HEADING" && msg.IsDouble())
		{
		m_heading = msg.GetDouble();
		m_IsHeading = true;
		}
	else  if(key == "NAV_DEPTH" && msg.IsDouble())
		{
		m_glebokoscZanurzenia = msg.GetDouble();
		m_isGlebokoscZanurzenia = true;
		}
	else  if(key == "NEW_OBSTACLE" && msg.IsString())
		{
		//cout<<msg.GetString()<<endl;
		vector<string> str_points = parseString(msg.GetString(), ',');
		XYPoint my_point(atof(str_points[0].c_str()), atof(str_points[1].c_str()));
		my_point.set_vz(m_glebokoscPodstawowa-atof(str_points[2].c_str()));
		m_przeszkody.push_back(my_point);
		}
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool DistanceToBottom::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool DistanceToBottom::Iterate()
{
	if(!m_run)
		{
		if(m_isX && m_isY && m_IsHeading && m_isGlebokoscZanurzenia)
			m_run = true;
		}
	else
		{
		wyznaczOdlOdDna();
		wykryjPrzeszkody();
		}
	
	return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool DistanceToBottom::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "AREA_DEPTH")
		m_glebokoscPodstawowa = atof(value.c_str());
	  else if(param == "NOISE")
		m_mi = atof(value.c_str());
	  else if(param == "NOISE_WIDTH")
		m_sigma = atof(value.c_str());
	  else if(param == "WIDTH_SONAR_VER")
		m_szerokoscWiazkiSonaruPionowa = atof(value.c_str());
	  else if(param == "WIDTH_SONAR_HOR")
		m_szerokoscWiazkiSonaruPozioma = atof(value.c_str());
	  else if(param == "RANGE_SONAR")
		m_zasiegSonaru = atof(value.c_str());
	  else if(param == "FILTER_LENGTH")
		m_dlugoscFiltru = atoi(value.c_str());
    }
  }
  
  wyznaczGlebokosc();
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void DistanceToBottom::RegisterVariables()
{
	Register("NAV_X", 0);
	Register("NAV_Y", 0);
	Register("NAV_HEADING", 0);
	Register("NAV_DEPTH", 0);
	Register("NEW_OBSTACLE", 0);
}

void DistanceToBottom::rozkodujPunkty(string depesza, vector<XYPoint>& punkty)
{
	biteStringX(depesza, '=');
	vector<string> str_points = parseString(depesza, ':');
	for(int i=0;i<str_points.size();i++)
		{
		vector<string> single_point = parseString(str_points[i], ',');
		if(single_point.size()==3)
			{
			XYPoint my_point(atof(single_point[0].c_str()), atof(single_point[1].c_str()));
			my_point.set_vz(atof(single_point[2].c_str()));
			punkty.push_back(my_point);
			}
		else if(single_point.size()==2)
			{
			XYPoint my_point(atof(single_point[0].c_str()), atof(single_point[1].c_str()));
			punkty.push_back(my_point);
			}
		}
}

/*void DistanceToBottom::rozkodujWysokoscPrzeszkod(string depesza)
{
	biteStringX(depesza, '=');
	vector<string> str_vector = parseString(depesza, ',');
	if(str_vector.size() > 0)
		{
		for(int i=0;i<str_vector.size();i++)
			{
			float wysokosc = atof(str_vector.at(i).c_str());
			m_wysokoscPrzeszkod.push_back(wysokosc);
			}
		}
}*/

void DistanceToBottom::wyznaczGlebokosc()
{
	for(int i=0;i<1000;i++)
		for(int j=0;j<1000;j++)
			{
			float val = ((1+sin(0.01*i))/1.2)*((1+cos(0.005*j))/2.0);
			m_glebokosc[i][j] = m_glebokoscPodstawowa-val; 
			}
}

void DistanceToBottom::wyznaczOdlOdDna()
{
	float los = fabs(normalRandom()*m_sigma+m_mi);
		
	int x = (int)m_currentPosition.x() + 500;
	int y = (int)m_currentPosition.y() + 500;
	
	float value = m_glebokosc[x][y] - m_glebokoscZanurzenia + los;
	if(value < 0)
		value = 0;
	
	value = filterOdlOdDna(value);
		
	//cout<<los<<" "<<value<<endl;
	Notify("DISTANCE_TO_BOTTOM",value);	
}

float DistanceToBottom::filterOdlOdDna(float input)
{
	if(m_filtrOdlOdDna.size() == 0)
		{
		for(int i=0;i<m_dlugoscFiltru;i++)
			m_filtrOdlOdDna.push_back(input);
		m_sumaFiltru = m_dlugoscFiltru*input;
		m_indeksOstatniego = 0;
		//cout<<"m_sumaFiltru="<<m_sumaFiltru<<" input="<<input<<endl;
		return input;
		}
	else
		{
		m_sumaFiltru = m_sumaFiltru - m_filtrOdlOdDna[m_indeksOstatniego];
		m_sumaFiltru = m_sumaFiltru + input;
		m_filtrOdlOdDna[m_indeksOstatniego] = input;
		float srednia = m_sumaFiltru/(float)m_dlugoscFiltru;
		m_indeksOstatniego++;
		m_indeksOstatniego = m_indeksOstatniego%m_dlugoscFiltru;
		//cout<<m_sumaFiltru<<" "<<m_indeksOstatniego<<endl;
		return srednia;
		}
}

void DistanceToBottom::wykryjPrzeszkody()
{
	vector<int> taSamaGlebokoscZasieg;
	//cout<<m_przeszkody.size()<<endl;
	for(int i=0;i<m_przeszkody.size();i++)
		{
		//cout<<m_przeszkody[i].z()<<" "<<m_glebokoscZanurzenia<<" "<<m_szerokoscWiazkiSonaruPionowa<<endl;
		if((m_glebokoscZanurzenia <= m_przeszkody[i].z() && (m_przeszkody[i].z() - m_glebokoscZanurzenia) <= m_szerokoscWiazkiSonaruPionowa)||
		(m_glebokoscZanurzenia > m_przeszkody[i].z()))
			{
			float dist = pow(pow(m_przeszkody[i].x() - m_currentPosition.x(),2)+pow(m_przeszkody[i].y() - m_currentPosition.y(),2),0.5);
			//cout<<dist<<endl;
			if(dist <= m_zasiegSonaru)
				taSamaGlebokoscZasieg.push_back(i);
			}
		}
	
	vector<int> indeksyPrzeszkod;
	for(int i=0;i<taSamaGlebokoscZasieg.size();i++)
		{
		XYPoint point = projectPoint(m_heading, m_zasiegSonaru, m_currentPosition.x(), m_currentPosition.y());
		float dist = distPointToSeg(m_currentPosition.x(), m_currentPosition.y(), point.x(), point.y(), m_przeszkody[taSamaGlebokoscZasieg[i]].x(), m_przeszkody[taSamaGlebokoscZasieg[i]].y());
		if(dist < m_szerokoscWiazkiSonaruPozioma)
			indeksyPrzeszkod.push_back(taSamaGlebokoscZasieg[i]);
		}
	
	for(int i=0;i<indeksyPrzeszkod.size();i++)
		{
		string wsp = doubleToString(m_przeszkody[indeksyPrzeszkod[i]].x()) + "," + doubleToString(m_przeszkody[indeksyPrzeszkod[i]].y()) + "," + doubleToString(m_przeszkody[indeksyPrzeszkod[i]].z());
		Notify("TRACKED_FEATURE",wsp);
		}
}


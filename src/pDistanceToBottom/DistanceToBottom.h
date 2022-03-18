/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: DistanceToBottom.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef DistanceToBottom_HEADER
#define DistanceToBottom_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "XYPoint.h"

using namespace std;

class DistanceToBottom : public CMOOSApp
{
 public:
   DistanceToBottom();
   ~DistanceToBottom();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables
	vector<XYPoint> m_przeszkody;
	
	float m_glebokosc[1000][1000];
	
	
	float m_glebokoscPodstawowa;
	XYPoint m_currentPosition;
	float m_heading;
	float m_glebokoscZanurzenia;
	
	bool m_isX;
	bool m_isY;
	bool m_IsHeading;
	bool m_isGlebokoscZanurzenia;
	bool m_run;
	float m_szerokoscWiazkiSonaruPionowa;
	float m_szerokoscWiazkiSonaruPozioma;
	float m_zasiegSonaru;
	
	float m_sigma;
    float m_mi;
	
	int m_dlugoscFiltru;
	vector<float> m_filtrOdlOdDna;
	float m_sumaFiltru;
	int m_indeksOstatniego;
	

 private: // State variables
 
 private: // Functions
	void rozkodujPunkty(string depesza, vector<XYPoint>& punkty);
	//void rozkodujWysokoscPrzeszkod(string depesza);
	void wyznaczGlebokosc();
	void wyznaczOdlOdDna();
	void wykryjPrzeszkody();
	float filterOdlOdDna(float input);
};

#endif 

/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_keepDistanceToBottom.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef keepDistanceToBottom_HEADER
#define keepDistanceToBottom_HEADER

#define DESIRED_DISTANCE 2
#define MAX_DEPTH 50
#define DEPTH_PRIORITY 50

#define  LICZBA_NEURONOW 20

#include <string>
#include "IvPBehavior.h"

typedef struct
{
	float wartosci[LICZBA_NEURONOW][LICZBA_NEURONOW];
	int liczbaWierszy;
	int liczbaKolumn;
}Macierz;

class NeuralNetwork
{
	Macierz* myNetwork;
	//float* wyjsciaNeuronow;
	float wyjsciaNeuronow[LICZBA_NEURONOW];
	int lWejsc;
	int lWyjsc;
	
	
	float sigmoidalna(int index, double parametr, Macierz *mac);
	float sigmoidalna1(int index, double parametr, Macierz *mac);
	float linear(int index, double parametr, Macierz *mac);
	float linear1(int index, double parametr, Macierz *mac);
	float linear2(int index, double parametr, Macierz *mac);
	void setLWejsc(int ile);
	void setLWyjsc(int ile);
	
public:

	NeuralNetwork(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc);
	NeuralNetwork(string myNetName, int liczbaWejsc, int liczbaWyjsc);
	NeuralNetwork();
	void setParameters(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc);
	NeuralNetwork(Macierz* myNet);
	 ~NeuralNetwork();
	void setWejscie(int nrWejscia, float wejscie);
	float getWyjscie(int nrWyjscia);
	void run();
	static Macierz* getMatrix(string myNetName);
};

class BHV_keepDistanceToBottom : public IvPBehavior {
public:
  BHV_keepDistanceToBottom(IvPDomain);
  ~BHV_keepDistanceToBottom() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions
	NeuralNetwork network;
	struct timeval m_start;
	struct timeval m_end;

    long m_mtime;
	long m_seconds;
	long m_useconds;
	bool m_first_time;
	
	float previous_difference;
	float difference;
	float suma_difference;
	float desired_distance;
	float max_depth;
	int priority;

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_keepDistanceToBottom(domain);}
}
#endif

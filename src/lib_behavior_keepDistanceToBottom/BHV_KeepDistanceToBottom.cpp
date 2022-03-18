/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_keepDistanceToBottom.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include <cmath>
#include <sys/time.h>
#include "XYPoint.h"
#include "AngleUtils.h"
#include <iostream>
#include <fstream>
#include "ZAIC_PEAK.h"
#include "BHV_KeepDistanceToBottom.h"

using namespace std;

NeuralNetwork::NeuralNetwork(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = myNet;
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork(string myNetName, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = getMatrix(myNetName);
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork(Macierz* myNet)
{
	myNetwork = myNet;
	lWejsc = -1;
	lWyjsc = -1;
}

void NeuralNetwork::setParameters(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = myNet;
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork()
{
	myNetwork = NULL;
	lWejsc = 0;
	lWyjsc = 0;
}

Macierz* NeuralNetwork::getMatrix(string myNetName)
{
	Macierz* myMacierz = new Macierz;
	ifstream file(myNetName);
	string str; 
	
	int row_number = 0;
	int column_number = -1;
	while (std::getline(file, str))
		{
		vector<string> values = parseString(str,' ');
		if(column_number == -1)
			column_number = values.size();
		for(int i=0;i<column_number;i++)
			myMacierz->wartosci[row_number][i] = atof(values[i].c_str());
		row_number++;
		}
	myMacierz->liczbaWierszy = row_number;
	myMacierz->liczbaKolumn = column_number;
	return myMacierz;
}

void NeuralNetwork::setLWejsc(int ile)
{
	lWejsc = ile;
}
	
void NeuralNetwork::setLWyjsc(int ile)
{
	lWyjsc = ile;
}

NeuralNetwork::~NeuralNetwork()
{
	//delete []wyjsciaNeuronow;
	//free(wyjsciaNeuronow);
}
	
void NeuralNetwork::setWejscie(int nrWejscia, float wejscie)
{
	if(nrWejscia < lWejsc)
		wyjsciaNeuronow[nrWejscia] = wejscie;
	else
		printf("!!!!!!!!!!!!!blad NeuralNetwork::setWejscie");
}
	
float NeuralNetwork::getWyjscie(int nrWyjscia)
{
	if(nrWyjscia < lWyjsc)
		//return wyjsciaNeuronow[myNetwork->liczbaWierszy-nrWyjscia-1];
		return wyjsciaNeuronow[myNetwork->liczbaWierszy-lWyjsc+nrWyjscia];
 	else
		printf("!!!!!!!!!!!!!blad NeuralNetwork::getWyjscie = %d",nrWyjscia);
	
	return 0;
}
	
void NeuralNetwork::run()
{
	for(int i=lWejsc;i<myNetwork->liczbaWierszy;i++)
		{
		float parameter = myNetwork->wartosci[i][myNetwork->liczbaWierszy+1];
		if(parameter==0)
			parameter=1;	
			
		wyjsciaNeuronow[i] = sigmoidalna(i,1,myNetwork);//sieci wyjsciowe
		}
}

float NeuralNetwork::sigmoidalna(int index, double parametr, Macierz *mac)
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return (2/(1+exp(-parametr*returnWartosc)))-1;
}

float NeuralNetwork::sigmoidalna1(int index, double parametr, Macierz *mac)
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return (1/(1+exp(-parametr*returnWartosc)));
}

float NeuralNetwork::linear(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return returnWartosc;
}

float NeuralNetwork::linear1(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	if(returnWartosc > 1)
		return 1;
	if(returnWartosc < -1)
		return -1;
	
	return returnWartosc;
}

float NeuralNetwork::linear2(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	if(returnWartosc > 1)
		return 1;
	if(returnWartosc < 0)
		return 0;
	
	return returnWartosc;
}

//---------------------------------------------------------------
// Constructor

BHV_keepDistanceToBottom::BHV_keepDistanceToBottom(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "KeepDistanceToBottom");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "depth");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("DISTANCE_TO_BOTTOM,NAV_DEPTH");
  
  m_first_time = true;
  suma_difference = 0;
  desired_distance = DESIRED_DISTANCE;
  max_depth = MAX_DEPTH;
  priority = DEPTH_PRIORITY;
  
  Macierz* mac = NeuralNetwork::getMatrix("macierz_depth.txt");
  network.setParameters(mac,2,1);
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_keepDistanceToBottom::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "desired_distance_to_bottom") && isNumber(val)) {
    desired_distance = double_val;
    return(true);
  }
  else  if((param == "max_depth") && isNumber(val)) {
    max_depth = double_val;
    return(true);
  }
  else  if((param == "depth_priority") && isNumber(val)) {
    priority = (int)double_val;
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_keepDistanceToBottom::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_keepDistanceToBottom::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_keepDistanceToBottom::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_keepDistanceToBottom::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_keepDistanceToBottom::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_keepDistanceToBottom::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_keepDistanceToBottom::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_keepDistanceToBottom::onRunState()
{
  // Part 1: Build the IvP function
  bool ok;
  double distanceToBottom = getBufferDoubleVal("DISTANCE_TO_BOTTOM", ok);
  if(!ok)
	return(0);
	
  double depth = getBufferDoubleVal("NAV_DEPTH", ok);
  if(!ok)
	return(0);

  if(distanceToBottom < 0 || depth > max_depth)
	  postEMessage("Wrong distance to bottom or depth of the vehicle, distanceToBottom=" + doubleToString(distanceToBottom) + " depth=" + doubleToString(depth)); //generowany jest blad i pojazd wchodzi w all-stop
	
  float newDepth = depth;
 
  gettimeofday(&m_end, NULL);
  
  if(m_first_time)
	{
	m_start = m_end;
	m_first_time = false;
	}
	
  m_seconds  = m_end.tv_sec  - m_start.tv_sec;
  m_useconds = m_end.tv_usec - m_start.tv_usec;

  m_mtime = ((m_seconds) * 1000 + m_useconds/1000.0) + 0.5;
  
  previous_difference = difference;
  difference = distanceToBottom -  desired_distance;
  suma_difference += difference;
  float pochodna = difference/previous_difference;
  
  network.setWejscie(0,previous_difference/MAX_DEPTH);
  network.setWejscie(1,difference/MAX_DEPTH);
  network.run();
  newDepth = depth + network.getWyjscie(0);
  
  //newDepth = newDepth + P*difference + I*suma_difference + D*pochodna;
  
  newDepth = newDepth + difference;
  
  if(newDepth > max_depth)
	  newDepth = max_depth;
  if(newDepth < 0)
	 newDepth = 0; 
	  
  if(fabs(difference) < 0.1)
	  suma_difference = 0;
		
	
	m_start = m_end;
	
	ZAIC_PEAK crs_zaic(m_domain, "depth");
	//crs_zaic.setSummit(40);
	//crs_zaic.setPeakWidth(3);
	//crs_zaic.setBaseWidth(100);
	//crs_zaic.setSummitDelta(50);  
	//crs_zaic.setValueWrap(true);
	double peak_width = newDepth / 2.0;
    crs_zaic.setParams(newDepth, peak_width, 1.6, 20, 0, 100);
	if(crs_zaic.stateOK() == false) {
		string warnings = "Depth ZAIC problems " + crs_zaic.getWarnings();
		postWMessage(warnings);
		return(0);
		}
	
    IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();
	if(!crs_ipf)
		postWMessage("Failure on the SPD ZAIC via ZAIC_PEAK utility");
	
	//string info = "depth:" + doubleToString(depth) + " distanceToBottom:" + doubleToString(distanceToBottom) + " newDepth:" + doubleToString(newDepth);
	//postMessage("OOOODUPA5", info);



  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(crs_ipf)
	crs_ipf->setPWT(priority);

  return(crs_ipf);
}


/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: ControlVehicleScanning.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef ControlVehicleScanning_HEADER
#define ControlVehicleScanning_HEADER

#include <sys/time.h>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

#define SPEED "speed"
#define PATH "points"

#define  LICZBA_NEURONOW 20
#define MAX_LEAD_DAMPER 1
#define TURN_RATE 70
#define MIN_LEAD_DISTANCE 2
#define THURN_LOSS 0.85
#define  MAX_ACCELERATION 0.5
#define  MAX_DECCELERATION 0.5
#define PROG_TEST 30
#define PROG_SYNCH_UCZ 3
#define PROG_TRACK_UCZ 2
#define  MAX_VEHICLE_SPEED 2
#define  SPEED_FACTOR 20
#define  RUDDER_FACTOR 1.2
#define MAX_THRUST 100
#define MAX_RUDDER 100

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

class ControlVehicleScanning : public AppCastingMOOSApp
{
 public:
   ControlVehicleScanning();
   ~ControlVehicleScanning();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
 NeuralNetwork network;
 double m_speed_turn;
 double m_speed_scanning;

 private: // State variables
 double m_heading;
 double m_speed;
 double m_dist_to_bottom;
 double position[2];
 bool m_scanning;
 bool m_run_mission;
 bool m_go_to_point;
 bool m_return;
 bool m_remote;
 bool m_path_set;
 double m_lead_damper;
 double m_turn_rate;
 double m_wsp_skalowania;
 double m_lead_distance;
 double m_wsp_thurn_loss;
 double m_thurn_loss;
 double m_wsp_acc;
 double m_wsp_dcc;
 double m_max_acceleration;
 double m_max_decceleration;
 double m_vehicle_param;
 double m_threshold_for_error_track;
 double m_threshold_for_error_time;
 double m_speed_factor;
 double m_rudder_factor;
 
 int m_no_of_waypoint;
 XYSegList m_list_of_waypoints;
 
 double maxBladSynchronizacjiError;
 double maxBladSynchronizacji;
 double maxBladTrackError;
 double maxBladTrack;
 int wayPoint;
 double previousErrorTrack;
 double previousErrorSynch;
 double diffInDisTime;
 double dist_to_trackline;
 double czas;
 bool start;
 double time_start_seconds;
 
 void getScanningParameters(string str);
 bool runControl();
 
};

#endif 

/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: SearchArea.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>

#include "MBUtils.h"
#include "SearchArea.h"
#include "SearchAreaTask.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SearchArea::SearchArea()
{
}

//---------------------------------------------------------
// Destructor

SearchArea::~SearchArea()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SearchArea::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SearchArea::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SearchArea::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SearchArea::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	

  //SearchArea test
  searchTask = new SearchAreaTask();
  searchTask->genTestTask(-10, -30, 35, 50, 30);
  searchTask->setStart_point(-50, -50);
  searchTask->setLaneWidth(4);
  searchTask->lawnMove();
  viewSearchTask();

  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void SearchArea::RegisterVariables()
{
  // Register("FOOBAR", 0);
}

void SearchArea::viewSearchTask() {

	  string marker = string("type=diamond,x=")
			  +to_string(searchTask->getStart_point().x())
			  +",y="+to_string(searchTask->getStart_point().y())
			  +",label=alpha,color=red,width=4.5";
	  Notify("VIEW_MARKER", marker);

	  XYPolygon polygon = searchTask->builArea("search area");
	  polygon.set_active(true);
	  polygon.set_edge_size(1);
	  polygon.set_edge_color("white");
	  Notify("VIEW_POLYGON", polygon.get_spec());

	  XYSegList segList1 = searchTask->buildSegList(0, "AUV1");
	  segList1.set_active(true);
	  segList1.set_edge_size(1);
	  segList1.set_edge_color("red");
	  segList1.set_label_color("red");
	  cout<<"SegList:"<<segList1.get_spec()<<endl;
	  Notify("VIEW_SEGLIST", segList1.get_spec());

	  segList1 = searchTask->buildSegList(1, "AUV2");
	  segList1.set_active(true);
	  segList1.set_edge_size(1);
	  segList1.set_edge_color("yellow");
	  segList1.set_label_color("yellow");
	  cout<<"SegList:"<<segList1.get_spec()<<endl;
	  Notify("VIEW_SEGLIST", segList1.get_spec());

	  segList1 = searchTask->buildSegList(2, "AUV3");
	  segList1.set_active(true);
	  segList1.set_edge_size(1);
	  segList1.set_edge_color("blue");
	  segList1.set_label_color("blue");
	  cout<<"SegList:"<<segList1.get_spec()<<endl;
	  Notify("VIEW_SEGLIST", segList1.get_spec());

}

//============================================================================
// Name        : ncrss2mmd.cpp
// Author      : Mark Meadows
// Version     : 0.0.1.1
// Copyright   : Copyright (c) 2019 Mark Meadows
// Description :nc2ss2md in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "tinyxml/tinyxml.h"

using namespace std;

void parse_xml(void); //Function to parse RSS feed from NextCloud

int main()
{

parse_xml(); //Parse XML from NextCloud


}


/*
========================================================================================
Function To Parse XML From NextCloud
All we need is the last piece of information published
========================================================================================
 */


void parse_xml(void)
{
	char test[250000];
		TiXmlElement *pelem;



		TiXmlDocument   doc("Test.xml");

		    if (doc.LoadFile())
		    {
		        TiXmlNode *elem = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("item");//|
		        pelem =elem->FirstChildElement("title");												  //|This gets the First Element
		        if (pelem) strcpy(test, (char*) pelem->GetText());										  //|From the RSS Feed under <title>
		        printf(" %s\n",test);																	  //|the event that happened

		        pelem =elem->FirstChildElement("pubDate");                                                //|
		        if (pelem) strcpy(test, (char*) pelem->GetText());                                        //|This gets the next Element
		        printf(" %s\n",test);                                                                     //|  The published date and time

		    }

return;

}
/*
=============================================================================================
End of XML Parsing Function
=============================================================================================
 */









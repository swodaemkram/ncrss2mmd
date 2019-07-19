//============================================================================
// Name        : ncrss2mmd.cpp
// Author      : Mark Meadows
// Version     : 0.0.1
// Copyright   : Copyright (c) 2019 Mark Meadows
// Description :nc2ss2md in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "tinyxml/tinyxml.h"



using namespace std;

int main()
{

		char test[250000];
	    TiXmlDocument doc("Test.xml");
		doc.LoadFile();

// *******    We have opened the XML file and are ready to run through it   **********

		      TiXmlElement *root, *pelem, *elem ;

		       root = doc.FirstChildElement();

		       //for(elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())


		       for (elem = root->FirstChildElement("channel"); elem != NULL; elem = elem->NextSiblingElement())

		       {

		    	  	 // pelem = elem->FirstChildElement("title");

		    	   	   pelem =elem->FirstChildElement("title");
		    	       if (pelem) strcpy(test, (char*) pelem->GetText());
		    		   printf("The Data 1 = %s\n",test);

		    		   pelem = elem->FirstChildElement();
		    		   if (pelem) strcpy(test, (char*) pelem->GetText());
		    		   printf("The Data 2 = %s\n",test);

		       }



}













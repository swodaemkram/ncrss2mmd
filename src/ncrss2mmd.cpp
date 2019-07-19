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

int main()
{

	char test[250000];
	TiXmlElement *pelem;



	TiXmlDocument   doc("Test.xml");

	    if (doc.LoadFile())
	    {
	        TiXmlNode *elem = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("item");
	        pelem =elem->FirstChildElement("title");
	        if (pelem) strcpy(test, (char*) pelem->GetText());
	        printf(" %s\n",test);

	        pelem =elem->FirstChildElement("pubDate");
	        if (pelem) strcpy(test, (char*) pelem->GetText());
	        printf(" %s\n",test);



	    }










}













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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <curl/curl.h>

using namespace std;

void parse_xml(void); //Function to parse RSS feed from NextCloud
void get_nextcloud_rssfeed(void);
void send_data_to_mattermost(void);
void read_config(void);

char RssURL[250] = {};       //A place to put the Rss Feeds URL
char WebHookURL[250] = {};	 //A Place to put the WebHookURL
char Filter[250] = {};       //A Place for the Filter Infosize_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
char NewMessageFromRSSFeed[1024] = {};
char OldMessageFromRSSFeed[1024] = {};
char SendToWebHook[2048] = {};

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}


int main()
{

read_config();
//DEBUG printf("RssURL = %s\n",RssURL);         //Debug
//DEBUG printf("WebHookURL = %s\n",WebHookURL); //Debugstring data((const char*) ptr, (size_t) size * nmemb);
//DEBUG printf("Filter = %s\n",Filter);			//Debug

get_nextcloud_rssfeed();    //Download the Rss Feed from NextCloud and hand it off to the parser
parse_xml();                //Parse RSS Feed XML from NextCloud (completed)
send_data_to_mattermost();  //Send the gleaned data to MatterMost Server Via Web Hook

}
/*
========================================================================================
Function To Parse RSS feed XML From NextCloud
All we need is the last piece of information published
========================================================================================
 */
void parse_xml(void)
{
char test[250];
TiXmlElement *pelem;
TiXmlDocument   doc("Test.xml");

	    if (doc.LoadFile())
	    {
	        TiXmlNode *elem = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("item");//|
	        pelem =elem->FirstChildElement("title");												  //|This gets the First Element
	        if (pelem) strcpy(test, (char*) pelem->GetText());										  //|From the RSS Feed under <title>
	        printf("======================================================================\n");
	        printf("%s\n",test);																	  //|the event that happened
	        pelem =elem->FirstChildElement("pubDate");                                                //|
	        if (pelem) strcpy(test, (char*) pelem->GetText());                                        //|This gets the next Element
	        printf("%s\n",test);                                                                    //|  The published date and time
	        printf("======================================================================\n");
	    }

return;
}
/*
=============================================================================================
End of XML Parsing Function
=============================================================================================
Get The RSS Feed From Next Cloud
=============================================================================================
*/
void get_nextcloud_rssfeed(void)
{
	   FILE *fp = NULL;
	   fp = fopen("Test.xml","wb");

	   CURL *curl;
	   CURLcode res;
	   curl_global_init(CURL_GLOBAL_DEFAULT);
	   curl = curl_easy_init();
	   curl_easy_setopt(curl, CURLOPT_URL,RssURL);
	   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  write_data);
	   curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	   res = curl_easy_perform(curl);

	   if(res != CURLE_OK)
	   {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
	    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
       }

	   curl_global_cleanup();
	   fclose(fp);

	   return;

}
/*
===============================================================================================
The End of Getting The RSS Data from NextCloud
===============================================================================================
*/

void send_data_to_mattermost(void)
{
	return;
}
/*
============================================================================================
Lets read the Config File and Load it
============================================================================================
 */
void read_config(void)
{
	//With only a few lines in the config file we will just make a conf "text" file
	//We will put it in the normal place /etc and we will call it ncrssmmd.conf
	FILE *Config_File = NULL;                        // declare config file Pointer

		 		Config_File = fopen("/etc/ncrss2mmd.conf", "r");  	// Open config file
		 		if (Config_File == NULL){
		 			printf("Could not open Config File\n");
		 			exit(1);
		 		}

		 		fscanf(Config_File,"%s", RssURL);
		 		fscanf(Config_File,"%s", WebHookURL);
		 		fscanf(Config_File,"%s", Filter);
		 		fclose(Config_File);

	return;
}
/*
============================================================================================
Config File Loaded
============================================================================================
*/

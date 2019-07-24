//============================================================================
// Name        : ncrss2mmd.cpp
// Author      : Mark Meadows
// Version     : 0.98.0.1
// Copyright   : Copyright (c) 2019 Mark Meadows
// Description :nc2ss2md in C++, Ansi-style
//============================================================================

#include <string>
#include "tinyxml/tinyxml.h"
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

void parse_xml(void); //Function to parse RSS feed from NextCloud
void get_nextcloud_rssfeed(void);
void send_data_to_mattermost(void);
void read_config(void);
void log_function(string log_message);

char RssURL[250] = {};       //A place to put the Rss Feeds URL
char WebHookURL[250] = {};	 //A Place to put the WebHookURL
char Filter[250] = {};       //A Place for the Filter
char RSSTime[250] = {};      //RSS Time information
char NewMessageFromRSSFeed[1024] = {};
char OldMessageFromRSSFeed[1024] = {};
char SendToWebHook[2048] = {};
char SentFromWhom[250] = {};
string logmessage = "";
string Version = "ncrss2mmd Ver. 0.98.0";
string ByWho = "By Mark Meadows";

struct curl_slist *headers = NULL;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main()
{
read_config();//(completed)

logmessage = "ncrss2mmd has started";
log_function(logmessage);
logmessage ="";

while(1)							//This is going to be a service so forever loop
	{
		get_nextcloud_rssfeed();    //Download the Rss Feed from NextCloud and hand it off to the parser(completed)
		parse_xml();                //Parse RSS Feed XML from NextCloud (completed)

		if (strcmp(NewMessageFromRSSFeed,OldMessageFromRSSFeed) != 0)
		{
			char * isinthere = {};
			//printf("NewMessageFromRSSFeed = %s Filter = %s",NewMessageFromRSSFeed,Filter);
			isinthere = strstr(NewMessageFromRSSFeed,Filter);
			//printf("isinthere = %s\n",isinthere);
			if (isinthere != NULL)
			{
				log_function(NewMessageFromRSSFeed);
				send_data_to_mattermost();  //Send the gleaned data to MatterMost Server Via Web Hook(in progress)
			}
		}


		sleep(5);					//Speed of checking RSS Feed set for every 5 seconds
	}
}
/*
========================================================================================
Function To Parse RSS feed XML From NextCloud
All we need is the last piece of information published
========================================================================================
 */
void parse_xml(void)
{
TiXmlElement *pelem;
TiXmlDocument   doc("temp.xml");

	    if (doc.LoadFile())
	    {
	        TiXmlNode *elem = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("item");//|
	        pelem =elem->FirstChildElement("title");												  //|This gets the First Element
	        if (pelem) strcpy(NewMessageFromRSSFeed, (char*) pelem->GetText());						  //|From the RSS Feed under <title>
	        //printf("======================================================================\n");
	        //printf("%s\n",NewMessageFromRSSFeed);													  //|the event that happened
	        pelem =elem->FirstChildElement("pubDate");                                                //|
	        if (pelem) strcpy(RSSTime, (char*) pelem->GetText());                                     //|This gets the next Element
	        //printf("%s\n",RSSTime);                                                                 //|  The published date and time
	        //printf("======================================================================\n");
	    }
	    remove("temp.xml");
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
	   fp = fopen("temp.xml","wb");

	   CURL *curl;
	   CURLcode res;
	   curl_global_init(CURL_GLOBAL_DEFAULT);
	   curl = curl_easy_init();
	   curl_easy_setopt(curl, CURLOPT_URL,RssURL);
	   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);// Dont Check SSL Cert (Not Secure !)
	   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);// Dont CHeck SSL Cert (Not Secure !)
	   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  write_data);
	   curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	   res = curl_easy_perform(curl);

	   if(res != CURLE_OK)
	   {
		logmessage = "Unable to get rss feed ....";
		log_function(logmessage);
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
       	curl_easy_cleanup(curl);
       	logmessage = "";
	   }

	   curl_global_cleanup();
	   fclose(fp);

	   return;

}
/*
===============================================================================================
The End of Getting The RSS Data from NextCloud
===============================================================================================
Lets Send Data To MatterMost
===============================================================================================
*/
void send_data_to_mattermost(void)
{

		   CURL *curl;
		   CURLcode res;
		   curl_global_init(CURL_GLOBAL_DEFAULT);
		   curl = curl_easy_init();

		   // Need to build the MatterMost URL to send the Data to the WebHook
		   //Sample Curl Command to post to mattermost
		   //curl -i -X POST -H 'Content-Type: applicati/json' -d '{"text": "This is a test of the Matermost web hook system "}' http://talk.kyin.net/hooks/6c78zsda4fy

           sprintf(SendToWebHook,"{\"text\": \"%s %s %s\"}",NewMessageFromRSSFeed,RSSTime,SentFromWhom); //We have to escape all the JSON crap!

		   curl_easy_setopt(curl, CURLOPT_URL,WebHookURL);
		   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //Dont Check SSL Cert.
		   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //Dont Check SSL Cert.
		   //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, R"anydelim( {"text": "You removed public link for DFocuments "} )anydelim");
		   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, SendToWebHook);
		   headers = curl_slist_append(headers, "Expect:");    //Set Header Types For JSON
		   headers = curl_slist_append(headers, "Content-Type: application/json"); //Set Header Type For JSON
		   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		   res = curl_easy_perform(curl);

		   if(res != CURLE_OK)
		   {
			logmessage = "Unable to Connect to Mattermost Server";
			log_function(logmessage);
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
		    curl_easy_strerror(res));
	        curl_easy_cleanup(curl);
	       }

		   curl_global_cleanup();
		   strcpy(OldMessageFromRSSFeed,NewMessageFromRSSFeed);
	return;
}
/*
============================================================================================
End of Sending Data To MatterMost
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
		 			logmessage = "Could not open configuration file";
		 			log_function(logmessage);
		 			printf("Could not open Config File\n");
		 			exit(1);
		 		}

		 		fscanf(Config_File,"%[^\n]\n", RssURL);      //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n", WebHookURL);	 //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n", Filter);		 //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n",SentFromWhom); //This will Read to the end of each line until a carriage return

		 		fclose(Config_File);

		 		log_function(Version);
		 		log_function(ByWho);
		 		logmessage = "";
		 		std::string logmessage1 = "The Config File Say the RssURL = ";
		 		std::string logmessage2 = RssURL;
		 		std::string logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the WebHookURL = ";
		 		logmessage2 = WebHookURL;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the Filter = ";
		 		logmessage2 = Filter;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the SentFromWhom = ";
		 		logmessage2 = SentFromWhom;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage = "Config File Loaded ...";
		 		log_function(logmessage);
		 		logmessage = "";

	return;
}
/*
============================================================================================
Config File Loaded
============================================================================================
Log Function
============================================================================================
*/

void log_function(string log_message)
{

		struct timespec ts;
	    timespec_get(&ts, TIME_UTC);
	    char buff[100];
	    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));

		 ofstream file;
		 file.open ("/var/log/ncrss2mmd.log",std::ios_base::app);

		char MyTime[27];
		sprintf(MyTime," %s.%09ld " , buff,ts.tv_nsec); //Format and apply data

		file<<MyTime;
		file<<log_message;
		file<<"\n";
		file.close();

		return;

}
/*
================================================================================================
End of the Log Function
================================================================================================
 */

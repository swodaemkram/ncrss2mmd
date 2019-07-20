# ncrss2mmd
Nextcloud to Mattermost Activity Monitor
I need a way to monitor my NextCloud server and know
when someone downloads something from it.

So when finished this will be a daemon that will live on the 
NextCloud server and monitor the RSS feed in question
and send this information to an incomming web-hook in
my Mattermost Server. That will show me who downloaded what
when in a mattermost channel.

This Daemon will be writen in c++ .

There will be a simple configuration file since there will
only be two options needed a RSS feed URL and a Mattermost 
Web-Hook URL so a simple text conf file in the /etc directory
named ncrss2mmd.conf. I think I will add a 3rd option for a 
filter. Since all activity is publised in the NextCloud RSS 
feed I should have a filter (example) "download" so I only 
get these actions in the Mattermost feed or if its blank 
we could see all activity.
===========================================================
Configuration File is complete and RSS XML parsing is 
complete.
===========================================================
Up next Network connection to NextCloud RSS Feed





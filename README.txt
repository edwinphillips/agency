This is a **very** early commit of a module for Estate Agency websites using Drupal.

The system does have a functional import for Rightmove BLM format files. 

Uses Custom Entities

What isn't done....

*	Views are not wired up properly
*	Rightmove import has not be tested to the point of satisfactions, but does appear to work
*	Very little commenting going on at the minute..sorry
*	The ajax mini forms to add pictures / media etc in the add / edit property form	are returning errors at the minute (array flip). Know what's causing it, just need to get my arse into gear and fix it
*	Ajax mini forms need to have the fields cleared properly.

What it does do is allow creating and management of property and listings, allows import via Rightmove BLM file, connects to FTP server to download remote BLM files and images and unzips those that are zipped. Please be warned it will probably break at the minute.


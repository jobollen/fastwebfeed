// $Id$
---------------------------
-- CONTENTS OF THIS FILE --
---------------------------
* Introduction
  - SimpleUpdateProtocol (SUP)
  - PubSubHubbbub
* Summary
  - fastwebfeed.module
  - fastwebfeed_sup.module
  - fastwebfeed_pubsubhubbub.module
  - fastwebfeed_aggregator.module
  - fastwebfeed_taxonomy.module
  - fastwebfeed_commentrss.module
* Installation
* Configuration
* Contact 



------------------
-- Introduction --
------------------

When the Publisher insert a new node then your RSS feeds will be updated. 
The Fastwebfeed module pings the Service(s)/Hub(s) saying that there's an update.
It's a simple way to let people know in real-time when your feed is updated using the SimpleUpdateProtocol and PubSubHubbub. 

This Module supports:
* SimpleUpdateProtocol (SUP)
* PubSubHubbub
* PubSubHubbub and SUP in the "rss.xml"
* PubSubHubbub and SUP in the "aggregator" feeds
* PubSubHubbub and SUP in the "taxonomy" feeds
* PubSubHubbub and SUP in the "comment" feeds

Please contact me if you have issues/features requests.
Current maintainer: Jo Bollen (JoBo) http://drupal.org/user/111415
Documentation: http://cupid-project.be/project/fastwebfeed-module

Licensed under the GNU/GPL License


SimpleUpdateProtocol (SUP)
==========================
SUP (Simple Update Protocol) is a simple and compact "ping feed" that web services can produce in order to alert the consumers of their feeds when a feed has been updated. 
This reduces update latency and improves efficiency by eliminating the need for frequent polling.
Simple Update Protocol (SUP) is quickly gaining adoption and is already being used by Friendfeed, ...

For more information about the Simple Update Protocol (SUP) protocol, please visit: 
* http://code.google.com/p/simpleupdateprotocol/


PubSubHubbub
============
PubSubHubbub is a simple, open, web-hook-based pubsub (publish/subscribe) protocol.
Parties (servers) speaking the PubSubHubbub protocol can get near-instant notifications (via webhook callbacks) when a topic (feed URL) they're interested in is updated.
PubSubHubbub is quickly gaining adoption and is already being used by Google (GTalk), SuperFeedr, ...

For more information about the PubSubHubbub protocol, please visit: 
* http://code.google.com/p/pubsubhubbub/



-------------
-- SUMMARY --
-------------

fastwebfeed.module
==================
invoke PubSubHubbub and Sup in the default feed
* "rss.xml"


fastwebfeed_sup.module
==========================
* supports multiple SUP services
* pings your SUP services every time a feed is updated
* includes the SUP-ID HTTP header on your RSS feed, e.g. X-SUP-ID: http://friendfeed.com/api/public-sup.json#SUP-ID
* announces which SUP services you are using by adding <atom:link rel=" ...> declarations to your feed.
* adds <atom:link ...> to your RSS feeds along with the necessary XMLNS declaration for RSS 0.92/1.0

By default this plugin will ping the following services:
* http://friendfeed.com/api/public-sup-ping [Friendfeed]


fastwebfeed_pubsubhubbub.module
===============================
* supports multiple hubs
* pings your hubs every time a feed is updated
* announces which hubs you are using by adding <link rel="hub" ...> declarations to your feed
* adds <atom:link rel="hub" ...> to your RSS feeds along with the necessary XMLNS declaration for RSS 0.92/1.0

By default this plugin will ping the following hubs:
* http://pubsubhubbub.appspot.com [Demo hub on Google App Engine]
* http://superfeedr.com/hubbub [SuperFeedr]


fastwebfeed_aggregator.module
=============================
you need to activate the cron see "requirements"
invoke PubSubHubbub and Sup in the "aggregator" feeds
* /aggregator/rss
* /aggregator/rss/? (?=category id)


fastwebfeed_taxonomy.module
=============================
invoke PubSubHubbub and Sup in the "taxonomy" feeds 
* /taxonomy/term/?/0/feed (?=term id)


fastwebfeed_commentrss.module
=============================
Invokes PubSubHubbub and Sup in the "comment" feeds
* crss
* crss/node/? (?=node id)
* crss/term/? (?=term id)



------------------
-- REQUIREMENTS --
------------------

* fastwebfeed_pubsubhubbub.module
  - Copy the publisher.php file from PHP library (http://code.google.com/p/pubsubhubbub/source/browse/trunk/publisher_clients/php/library/publisher.php)
    to sites/all/module/fastwebfeed/fastwebfeed_pubsubhubbub/includes/
    
* fastwebfeed_aggregator.module
  - you must activate the cron or run it manual 
    * Configuring cron jobs (http://drupal.org/cron)
    * poormanscron.module (http://drupal.org/project/poormanscron)
  - depends on the aggregator.module (core)
  
* fastwebfeed_taxonomy.module
  - depends on the taxonomy.module (core)

* fastwebfeed_commentrss.module
  - depends on the commentrss.module (http://drupal.org/project/commentrss)
  
------------------
-- INSTALLATION --
------------------

* Copy the Fastwebfeed module directory to your sites/all/modules directory.

* (required) Enable the "fastwebfeed.module" at Administer >> Site building >> Modules >> Fastwebfeed   (http://example.com/admin/build/modules#edit-status-fastwebfeed-wrapper)
* (required) Enable the "fastwebfeed.pubsubhubbub.module" at Administer >> Site building >> Modules >> Fastwebfeed PubSubHubbub   (http://example.com/admin/build/modules#edit-status-fastwebfeed-pubsubhubbub-wrapper)
* (required) Enable the "fastwebfeed_sup.module" at Administer >> Site building >> Modules >> Fastwebfeed SimpleUpdateProtocol (SUP)   (http://example.com/admin/build/modules#edit-status-fastwebfeed-sup-wrapper)
  
* (optional) Enable the "fastwebfeed_aggregator.module" at Administer >> Site building >> Modules >> Fastwebfeed (aggregator.module)   (http://example.com/admin/build/modules#edit-status-fastwebfeed-aggregator-wrapper)
* (optional) Enable the "fastwebfeed_commentrss.module" at Administer >> Site building >> Modules >> Fastwebfeed (commentrss.module)   (http://example.com/admin/build/modules#edit-status-fastwebfeed-commentrss-wrapper)
* (optional) Enable the "fastwebfeed_taxonomy.module" at Administer >> Site building >> Modules >> Fastwebfeed (taxonomy.module)   (http://example.com/admin/build/modules#edit-status-fastwebfeed-taxonomy-wrapper)

* If desired, configure the module at Administer >> Site configuration >> Fastwebfeed



-------------------
-- CONFIGURATION --
-------------------
  
* see the help guid at Administer >> Help >> Fastwebfeed
  (http://example.com/admin/help/fastwebfeed)



-------------
-- CONTACT --
-------------

Current maintainer(s):
* Jo Bollen (JoBo) http://drupal.org/user/111415

Documentation: 
* http://cupid-project.be/project/fastwebfeed-module
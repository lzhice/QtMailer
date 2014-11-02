QtMailer
========

Some Qt-classes for sending mails using an SMTP-server.

Since Qt is very feature rich, but misses classes for sending mails over a smtp-server I decided to start writing some. Not very pretty but running for me.

FEATURES
--------
* Send mails over an open smtp-server
* Send mails over ssl-connections (using SSL oder STARTTLS)
* Send mails over a server needing LOGIN
* Send mails with attachments (even multiple attachments)
* Send mails using multiple recepients in To:, Cc: or Bcc
* Can accept self signed certificates

NOT implemented yet
-------------------
* Any other AUTH-Method than LOGIN
* A lot more...

TODO
----
Documentation and of course anything listed in "NOT implemented yet"

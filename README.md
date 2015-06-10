QtMailer
========
A small library in Qt for sending mails using an SMTP-server.

Since Qt is very feature rich, but misses classes for sending mails over
a smtp-server I decided to start writing some. Not very pretty but
running for me.

FEATURES
--------
* Send mails over an open smtp-server
* Send mails over ssl-connections (using SSL oder STARTTLS)
* Send mails over a server needing LOGIN
* Send mails with attachments (even multiple attachments)
* Send mails using multiple recepients in To:, Cc: or Bcc:
* Can accept self signed certificates

NOT implemented yet
-------------------
* Any other AUTH-Method than LOGIN
* A lot more...

TODO
----
Documentation, more examples and of course anything listed in "NOT
implemented yet".

BUILDING
--------
The directory *src* holds the sources for QtMailer. You can build it
using qmake & make. The lib will be placed in the directory *lib*.
If you are running a Unix system you can then install the library and
header-files into your system. The library and headers will be installed
in */usr/lib* and */usr/include* by default.  You can change the PREFIX
(/usr) by using the PREFIX-Variable in qmake.

### Unix & Linux
    % cd src
    % qmake
    % make
    % make install

### FreeBSD
To install the files in /usr/local you have to add the alternative
prefix to qmake:

    % cd src
    % qmake PREFIX=/usr/local
    % make
    % make install

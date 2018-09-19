# Marble
Marble the Web Programming Language

To install visit: https://marble.wiki

# Building Dependencies
Marble requires the following libaries to be installed to compile

apache2-dev: Holds dependencies for building apache2 modules
    
libjsoncpp-dev: Used for file sessions
    
libcurl4-openssl-dev: Used for the network module

libmysqlclient-dev: Used for mysql communications

Debain:

sudo apt-get install apache2-dev

sudo apt-get install libjsoncpp-dev

sudo apt-get install libcurl4-openssl-dev

sudo apt-get install libmysqlclient-dev

# Building for Standalone
To build the terminal standalone application do the following commands

  sudo make clean

  sudo make config
  
  sudo make standalone-modules-install

Now you can use the command "marble"

# Building for Apache
 
 sudo make clean
 
 sudo make config
 
 sudo make apache2-install
 
 sudo service apache2 restart
 
 Please follow the install guide at https://marble.wiki to learn how to setup the Marble apache module.

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
 
 # Contribution Rules
 Marble has contribution rules that all contributers need to follow or risk having their pull request rejected.
 ## Basic Rules
 1. For new features you must create pull requests to the "dev" branch not the "master" branch.
 2. For bug fixes please submit pull requests directly to the master if the bug is present in the master branch
 3. Ask Daniel: dansblackberry2@hotmail.co.uk or founder@marble.wiki about implementing a feature if you wish to create a feature for Marble such as a new Marble library. Failure to ask for permission may result in your pull request not being wanted meaning you would of wasted your time. (Please email both of those accounts so we can reply to you as fast as possible)
 4. You may only add your name to the contributors file if you have (fixed a bug or created a new library) fixing typos or bad grammer does not grant you a place in the contributors file although we thank you for noticing this problem and reporting it to us.
 ## Coding Rules
 1. You must follow use braces in this fashion
 ```
 void abc()
 {
 
 }
 ```
 2. When creating libraries ensure you follow a simular pattern as is shown in existing libraries. All native Marble classes you develop for your Marble native library must have a "registerClass" static method that is responsible for registering the Marble class in your library. This static method should be inside the "c++" file that is representing the Marble native class you are creating
 3. You must use Makefiles. Pull request's that build without the use of Makefiles may be rejected
 4. In each class and function you register into the Marble interpreter you must provide comments stating the function prototype, class prototype, what class a function uses, weather or not it can also work alone without the class. You can see how this should be structured in pretty much every Marble library in the system. Ensure you stay to this structure as we have programs that automatically generate the marble documentation for the web.
 ## Legal rules
 Marble is under the General Public License V2 as long as you follow this license you are in your right to use Marble.

# HTTPServer

This is an http server written under OS X. It serves GET and POST (CGI) requests. HTTP version - 1.1

## Installation
Clone the repository
```
git clone https://github.com/kanes115/HTTPServer.git
```
Then go to the project
```
cd HTTPServer
```
Create a build folder
```
mkdir build
cd build
```
And make the project
```
cmake ..
make
```
In "out" project you have your server.

## Configuration
Server is configured from a file. You pass path to the file in an argument when running server.
```
./serverRunner <path-to-configuration>
```
In config file you specify some of the properties you want to start a server with. You can start a comment with "#".
Configuraton file should look like this:
```
#port number
port: 8009
# thats what is sent at "/"
root: example.html
# Max amount of clients connected
maxClients: 5
# Timeout after which we disconnect from a client
timeout: 15
# File to which we write logs
logFile: log.txt
# page that will be loaded when 404 accours
fileNotFoundPage: filenotfound.html
```
## Resources
You add resources you want to be availible for clients in a folder where your server runs.
They reach them by specyfying the path to them.
```
GET
<address>:<port>/path/to/resource/*.html
```
GET requests will only send a page to a client (or other data at a resource).
```
POST
<address>:<port>/path/to/the/resource/*.cgi
```
POST rquests will run a program *.cgi and send to client whatever it will send to stdout (see CGI)

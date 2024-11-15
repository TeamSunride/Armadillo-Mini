
// Load Wi-Fi library
#include <WiFi.h>

using namespace std;

// Replace with your network credentials
const char *ssid = "Armadillo-Mini SN1";
const char *password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
bool output26State = false;
bool output27State = false;

// Assign output variables to GPIO pins
const int output26 = 4;
const int output27 = 13;

void setup()
{
    Serial.begin(115200);
    // Initialize the output variables as outputs
    pinMode(output26, OUTPUT);
    pinMode(output27, OUTPUT);
    // Set outputs to LOW
    digitalWrite(output26, LOW);
    digitalWrite(output27, HIGH);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password, 1, 0, 1);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
}

void loop()
{
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    {                                  // If a new client connects,
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // turns the GPIOs on and off
                        if (header.indexOf("POST /26/on") >= 0)
                        {
                            Serial.println("GPIO 26 on");
                            output26State = true;
                            digitalWrite(output26, HIGH);
                        }
                        else if (header.indexOf("POST /26/off") >= 0)
                        {
                            Serial.println("GPIO 26 off");
                            output26State = false;
                            digitalWrite(output26, LOW);
                        }
                        else
                        {
                            Serial.println("GET /");
                            // Display the HTML web page
                            client.println("<!DOCTYPE html><html>");
                            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            client.println("<link rel=\"icon\" href=\"data:,\">");
                            // JavaScript
                            client.println("<script type=\"text/javascript\">function submit(endpoint,callback){var xhr=new XMLHttpRequest();xhr.onreadystatechange=function(){if(xhr.readyState===4&&xhr.status===200){alert(xhr.statusText);");
                            client.println("callback(xhr.status)}};xhr.open('post',`http://192.168.4.1${ endpoint }`,true);xhr.send()}function requestArm(){var r=(Math.random()+1).toString(36).substring(7);");
                            client.println("var response=prompt(`!------ WARNING! ------!\n\nARE YOU SURE YOU WANT TO ARM??????????\n\nEnter '${ r }' to confirm:`);if(response==r){var confirmed=confirm(`!------ DANGER! ------!\n\nYou are about to ARM`);");
                            client.println("submit(`/26/on`,res=>{if(res==200){setArmed()}})}else{alert(`The code you entered was not correct.\n\nAborting.`)}}function disarm(){submit(`/26/off`,res=>{if(res==200){setDisarmed()}})}");
                            client.println("function setArmed(){document.getElementById(\"status\").className=\"danger statusIndicator\";document.getElementById(\"status\").textContent=\"ARMED\";document.getElementById(\"toggle\").className=\"green\";");
                            client.println("document.getElementById(\"toggle\").textContent=\"CLICK TO DISARM\";document.getElementById(\"toggle\").onclick=disarm;}function setDisarmed(){document.getElementById(\"status\").className=\"noDanger statusIndicator\";");
                            client.println("document.getElementById(\"status\").textContent=\"NOT ARMED\";document.getElementById(\"toggle\").className=\"danger\";document.getElementById(\"toggle\").textContent=\"CLICK TO ARM\";document.getElementById(\"toggle\").onclick=requestArm;}</script>");
                            // CSS styles
                            client.println("<style>html{margin:auto;text-align:center;font-family:Arial, Helvetica, sans-serif}.danger{color:white;background-color:red}");
                            client.println(".noDanger{color:white;background-color:grey}.green{color:white;background-color:green}.statusIndicator{padding:4rem;font-size:500%}");
                            client.println("button{padding:1rem;border-radius:30px;border:none;font-size:150%}</style>");
                            // Web Page Heading
                            client.println("<body><h1>Armadillo Mini</h1><h4>v1.0</h4>");

                            if (output26State == true)
                                client.println("<main><hr><div id=\"status\" class=\"danger statusIndicator\">ARMED</div><hr><button id=\"toggle\" class=\"green\" onclick=\"disarm()\">CLICK TO DISARM</button></main>");
                            else
                                client.println("<main><hr><div id=\"status\" class=\"noDanger statusIndicator\">NOT ARMED</div><hr><button id=\"toggle\" class=\"danger\" onclick=\"requestArm()\">CLICK TO ARM</button></main>");

                            client.println("</body></html>");

                            // The HTTP response ends with another blank line
                            client.println();
                        }

                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


int left = 0;
int right = 1;

int xL = 0;
int yL = 0;
int xR = 0;
int yR = 0;

ESP8266WebServer server(80);

IPAddress apIP(192, 168, 0, 1);

void setup() {
  ESP.eraseConfig();
  
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  WiFi.softAP("tacotank");

  IPAddress myIP = WiFi.softAPIP();

  Serial.begin(300);

  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.begin();

  Serial.println("Started");
  
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();        
  
  motorRun(left, -yL*5);
  motorRun(right, -yR*5);

  //Serial.println(-yL*5);
}

void handleRoot(){
  server.send(200, "text/html", "<script type=\"text/javascript\">\n"
"  \n"
" var xL = 0;\n"
" var yL = 0;\n"
" var xR = 0;\n"
" var yR = 0;\n"
"\n"
" var xLS = 0;\n"
" var yLS = 0;\n"
"\n"
" var xRS = 0;\n"
" var yRS = 0;\n"
"\n"
" var left;\n"
" var right;\n"
" var l;\n"
" var r;\n"
" \n"
" var lastXL = 0;\n"
" var lastyL = 0;\n"
" var lastXR = 0;\n"
" var lastYR = 0;\n"
"\n"
" window.onload=function(){\n"
"   left = document.getElementById(\"left\");\n"
"   right = document.getElementById(\"right\");\n"
"\n"
"   l = left.getContext(\"2d\");\n"
"   r = right.getContext(\"2d\");\n"
"\n"
"   left.addEventListener(\"touchmove\", touchL, false);\n"
"   right.addEventListener(\"touchmove\", touchR, false);\n"
"\n"
"   left.addEventListener(\"touchend\", function(){\n"
"     xL = left.width/2;\n"
"     yL = left.height/2;\n"
"   }, false);\n"
"   right.addEventListener(\"touchend\", function(){\n"
"     xR = right.width/2;\n"
"     yR = right.height/2;\n"
"   }, false);\n"
"\n"
"\n"
"   left.addEventListener(\"touchstart\", function(e){\n"
"     xLS = e.targetTouches[0].screenX;\n"
"     yLS = e.targetTouches[0].screenY;\n"
"   }, false);\n"
"   right.addEventListener(\"touchstart\", function(e){\n"
"     xRS = e.targetTouches[0].screenX;\n"
"     yRS = e.targetTouches[0].screenY;\n"
"   }, false);\n"
"\n"
"   xL = left.width/2;\n"
"   yL = left.height/2;\n"
"\n"
"   xR = right.width/2;\n"
"   yR = right.height/2;\n"
"\n"
"   setInterval(run, 1000/60);\n"
"   setInterval(send, 1000/5);\n"
"\n"
" }\n"
"\n"
" function run(){\n"
"\n"
"\n"
"\n"
"   l.clearRect(0, 0, left.width, left.height);\n"
"   l.beginPath();\n"
"   l.arc(xL,yL,40,0,2*Math.PI);\n"
"   l.fillStyLe = \"#2962FF\";\n"
"   l.shadowBlur = \"10\";\n"
"   l.shadowColor = \"black\";\n"
"   l.fill();\n"
"\n"
"   r.clearRect(0, 0, right.width, right.height);\n"
"   r.beginPath();\n"
"   r.arc(xR,yR,40,0,2*Math.PI);\n"
"   r.fillStyLe = \"#2962FF\";\n"
"   r.shadowBlur = \"10\";\n"
"   r.shadowColor = \"black\";\n"
"   r.fill();\n"
" }\n"
"\n"
" function send(){\n"
"\n"
"   if(lastXL != xL || lastXR != xR || lastyL != yL || lastYR != yR){\n"
"     var xhr = new XMLHttpRequest();\n"
"     xhr.open('PUT', \"./control?xl=\"+(xL-left.width/2)+\"&yL=\"+(yL-left.height/2)+\"&xr=\"+(xR-right.width/2)+\"&yr=\"+(yR-right.height/2))\n"
"     xhr.send();\n"
"   }\n"
"   lastXL = xL;\n"
"   lastXR = xR;\n"
"   lastyL = yL;\n"
"   lastYR = yR;\n"
" \n"
" }\n"
"\n"
"\n"
" function touchL(e){\n"
"   e.preventDefault();\n"
"\n"
"   xL = e.targetTouches[0].screenX - xLS + left.width/2;\n"
"   yL = e.targetTouches[0].screenY - yLS + left.height/2;\n"
" }\n"
"\n"
" function touchR(e){\n"
"   e.preventDefault();\n"
"\n"
"   xR = e.targetTouches[0].screenX - xRS + right.width/2;\n"
"   yR = e.targetTouches[0].screenY - yRS + right.height/2;\n"
" }\n"
"\n"
"</script>\n"
"\n"
"<styLe>\n"
"\n"
" body {\n"
"   margin: 0;\n"
" }\n"
"\n"
" div {\n"
"   width: 100vw;\n"
"   height: 100vh;\n"
"   display: flex;\n"
"   flex-direction: row;\n"
"   flex-wrap: nowrap;\n"
"   background-color: #F3F3F3;\n"
" }\n"
"\n"
" canvas {\n"
"   flex-grow: 1;\n"
"   margin: 1%;\n"
"   border-radius: 5px;\n"
"   box-shadow: 1px 1px 3px grey;\n"
"   background-color: white;\n"
" }\n"
"\n"
"</styLe>\n"
"\n"
"<div>\n"
"\n"
" <canvas id=\"left\" styLe=\"margin-right: 0.5%\" width=\"485\" height=\"480\"></canvas>\n"
"\n"
" <canvas id=\"right\" styLe=\"margin-left: 0.5%\" width=\"485\" height=\"480\"></canvas>\n"
"\n"
"</div>\n");
  Serial.println("Connection");
}

void handleControl(){
  xL = server.arg(0).toInt();
  yL = server.arg(1).toInt();
  xR = server.arg(2).toInt();
  yR = server.arg(3).toInt();

  server.send(200, "text/plain", "");
  
}

void motorRun(int motor, int value){

  int maxSpeed = 950; // max speed motors can run to prevent burning motor controller

  if(value > maxSpeed)value = maxSpeed;
  if(value <-maxSpeed)value = -maxSpeed;

  if(motor == left){
    if(value > 0){
      analogWrite(D6, value);
      analogWrite(D7, 0);
    }else{
      analogWrite(D6, 0);
      analogWrite(D7, -value);
    }
  }
  
  if(motor == right){
    if(value > 0){
      analogWrite(D3, value);
      analogWrite(D4, 0);
    }else{
      analogWrite(D3, 0);
      analogWrite(D4, -value);
    }
  }
  
}

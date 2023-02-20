//#include <ESP8266WiFi.h>
//IPAddress ip(172, 20, 10, 11);                                           //IP
//IPAddress gateway(255, 20, 1, 17);                                       //шлюз
//IPAddress subnet(255, 255, 255, 0);                                      //маска
//WiFiServer server(80);                                                   //Создание объекта сервера
//
//const char* ssid     = "";                                                //логин WIFI
//const char* password = "";                                                //Пароль WIFI
//
//void wifiInit()
//{
//    //WIFI CONNECT
//  WiFi.mode(WIFI_STA);              //режим WIFI(клиент)
//  WiFi.begin(ssid, password);
//  WiFi.config(ip, gateway, subnet); //устаналиваем статический ip
//  server.begin();                   //запускааем сервер
//  
//  Serial.print("\nUse this URL : http://"); //вывод адреса в монитор порта
//  Serial.print(WiFi.localIP());
//  Serial.println("/");
//}

//void controller(String req,  WiFiClient client, int colorTemp1, int lux1, int colorTemp2, int lux2)
//{
//   if (req.indexOf("/Temp1") != -1){
//     updateTCS(SDA1, SCL1, colorTemp1, lux1);
//     Serial.println("Showing temperature");
//    }
//    else if(req.indexOf("/Lux1") != -1){
//       updateTCS(SDA1, SCL1, colorTemp1, lux1);
//       Serial.println("Showing Lux");
//    }
//    else if(req.indexOf("/Temp2") != -1){
//       updateTCS(SDA2, SCL2, colorTemp2, lux2);
//       Serial.println("Showing temperature");
//    }
//    else if(req.indexOf("/Lux2") != -1){
//       updateTCS(SDA2, SCL2, colorTemp2, lux2);
//       Serial.println("Showing Lux");
//    }
//    else {
//    Serial.println("invalid request");
//    client.stop();
//    return;
//    }
//}
//
//void loopClient(int colorTemp1, int lux1, int colorTemp2, int lux2)
//{
//    WiFiClient client = server.available();      // создание клиента, который соединяется с указанным IP
//    if (!client)return;
//
//    // Wait until the client sends some data
//    Serial.println("new client");
//    while(!client.available()){delay(1);}
//    
//    String req = client.readStringUntil('\r'); // Чтение строчки запроса
//    Serial.println(req);                       // Печать запрос
//    
//    controller(req, client);                   // Match the request
//    client.flush(); 
//   
//    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"; // Prepare the response
//    if (req.indexOf("/Temp1") != -1){
//      s += colorTemp1;
//    }
//    else if(req.indexOf("/Temp2") != -1)
//      s += colorTemp2;
//    
//    else if(req.indexOf("/Lux1") != -1)
//      s += lux1;
//    
//    else if(req.indexOf("/Lux2") != -1)
//      s += lux2;
//    
//    // Send the response to the client
//    client.print(s);
//    delay(1);
//    Serial.println("Client disonnected");
//  
//    // The client will actually be disconnected 
//    // when the function returns and 'client' object is detroyed
//}

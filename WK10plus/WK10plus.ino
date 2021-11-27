#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Deco wade";
const char* password = "abcd1234";
const char* host = "data.epa.gov.tw/";
const char* resource = "/api/v1/aqx_p_02?limit=1000&api_key=9be7b239-557b-4c10-9775-78cadfc555e9&sort=ImportDate%20desc&format=json/api/v1/aqx_p_02?limit=1000&api_key=9be7b239-557b-4c10-9775-78cadfc555e9&sort=ImportDate%20desc&format=json"; 
char jsonRead[600]; //讀取response後儲存JSON資料的變數，必須是全域變數
String json;

const char* host2 = "maker.ifttt.com"; //IFTTT server網址
const char* event = "line_notify";  //IFTTT事件名稱
const char* apiKey = "zmaeQJZUQLwROhEiBE7sI";  //IFTTT Applet keyconst char* host = "maker.ifttt.com"; //IFTTT server網址

const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\n"\
                      "MIIFsDCCA5igAwIBAgIQFci9ZUdcr7iXAF7kBtK8nTANBgkqhkiG9w0BAQUFADBe\n"\
                      "MQswCQYDVQQGEwJUVzEjMCEGA1UECgwaQ2h1bmdod2EgVGVsZWNvbSBDby4sIEx0\n"\
                      "ZC4xKjAoBgNVBAsMIWVQS0kgUm9vdCBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTAe\n"\
                      "Fw0wNDEyMjAwMjMxMjdaFw0zNDEyMjAwMjMxMjdaMF4xCzAJBgNVBAYTAlRXMSMw\n"\
                      "IQYDVQQKDBpDaHVuZ2h3YSBUZWxlY29tIENvLiwgTHRkLjEqMCgGA1UECwwhZVBL\n"\
                      "SSBSb290IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjANBgkqhkiG9w0BAQEF\n"\
                      "AAOCAg8AMIICCgKCAgEA4SUP7o3biDN1Z82tH306Tm2d0y8U82N0ywEhajfqhFAH\n"\
                      "SyZbCUNsIZ5qyNUD9WBpj8zwIuQf5/dqIjG3LBXy4P4AakP/h2XGtRrBp0xtInAh\n"\
                      "ijHyl3SJCRImHJ7K2RKilTza6We/CKBk49ZCt0Xvl/T29de1ShUCWH2YWEtgvM3X\n"\
                      "DZoTM1PRYfl61dd4s5oz9wCGzh1NlDivqOx4UXCKXBCDUSH3ET00hl7lSM2XgYI1\n"\
                      "TBnsZfZrxQWh7kcT1rMhJ5QQCtkkO7q+RBNGMD+XPNjX12ruOzjjK9SXDrkb5wdJ\n"\
                      "fzcq+Xd4z1TtW0ado4AOkUPB1ltfFLqfpo0kR0BZv3I4sjZsN/+Z0V0OWQqraffA\n"\
                      "sgRFelQArr5T9rXn4fg8ozHSqf4hUmTFpmfwdQcGlBSBVcYn5AGPF8Fqcde+S/uU\n"\
                      "WH1+ETOxQvdibBjWzwloPn9s9h6PYq2lY9sJpx8iQkEeb5mKPtf5P0B6ebClAZLS\n"\
                      "nT0IFaUQAS2zMnaolQ2zepr7BxB4EW/hj8e6DyUadCrlHJhBmd8hh+iVBmoKs2pH\n"\
                      "dmX2Os+PYhcZewoozRrSgx4hxyy/vv9haLdnG7t4TY3OZ+XkwY63I2binZB1NJip\n"\
                      "NiuKmpS5nezMirH4JYlcWrYvjB9teSSnUmjDhDXiZo1jDiVN1Rmy5nk3pyKdVDEC\n"\
                      "AwEAAaNqMGgwHQYDVR0OBBYEFB4M97Zn8uGSJglFwFU5Lnc/QkqiMAwGA1UdEwQF\n"\
                      "MAMBAf8wOQYEZyoHAAQxMC8wLQIBADAJBgUrDgMCGgUAMAcGBWcqAwAABBRFsMLH\n"\
                      "ClZ87lt4DJX5GFPBphzYEDANBgkqhkiG9w0BAQUFAAOCAgEACbODU1kBPpVJufGB\n"\
                      "uvl2ICO1J2B01GqZNF5sAFPZn/KmsSQHRGoqxqWOeBLoR9lYGxMqXnmbnwoqZ6Yl\n"\
                      "PwZpVnPDimZI+ymBV3QGypzqKOg4ZyYr8dW1P2WT+DZdjo2NQCCHGervJ8A9tDkP\n"\
                      "JXtoUHRVnAxZfVo9QZQlUgjgRywVMRnVvwdVxrsStZf0X4OFunHB2WyBEXYKCrC/\n"\
                      "gpf36j36+uwtqSiUO1bd0lEursC9CBWMd1I0ltabrNMdjmEPNXubrjlpC2JgQCA2\n"\
                      "j6/7Nu4tCEoduL+bXPjqpRugc6bY+G7gMwRfaKonh+3ZwZCc7b3jajWvY9+rGNm6\n"\
                      "5ulK6lCKD2GTHuItGeIwlDWSXQ62B68ZgI9HkFFLLk3dheLSClIKF5r8GrBQAuUB\n"\
                      "o2M3IUxExJtRmREOc5wGj1QupyheRDmHVi03vYVElOEMSyycw5KFNGHLD7ibSkNS\n"\
                      "/jQ6fbjpKdx2qcgw+BRxgMYeNkh0IkFch4LoGHGLQYlE535YW6i4jRPpp2zDR+2z\n"\
                      "Gp1iro2C6pSe3VkQw63d4k3jMdXH7OjysP6SHhYKGvzZ8/gntsm+HbRsZJB/9OTE\n"\
                      "W9c3rkIO3aQab3yIVMUWbuF6aC74Or8NpDyJO3inTmODBCEIZ43ygknQW/2xzQ+D\n"\
                      "hNQ+IIX3Sj0rnP0qCglN6oH4EZw=\n"\
                      "-----END CERTIFICATE-----\n";
                      //真ㄉ謝謝唐同學讓我發現我爬文了一整天原來只是憑證抓錯檔案，幹




void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
}

void loop(){

    //以char*格式儲存回傳的json
    //char* json = GetWeatherData();
    get();
    Serial.println(json);
    
    //將字串轉換為JSON，儲存在doc中
    //StaticJsonDocument<600> doc;
    DynamicJsonDocument doc(json.length()*2);
    deserializeJson(doc, json);
    
    String location = doc["records"][1]["Site"]; 
    //const char* weather = doc["weather"][0]["description"];
    String PM25 = doc["records"][1]["PM25"];
    double PM25i = PM25.toDouble();
    String a,b;
    
    Serial.print("*** ");
    Serial.print(location);
    Serial.println("現在天氣 ***");
    Serial.print("PM2.5數值: ");
    Serial.println(PM25);
    if(PM25i<=50){
      a="%e8%89%af%e5%a5%bd";
      b="良好";
    }
    else if(PM25i<=100){
      a="%e6%99%ae%e9%80%9a";
      b="普通";
    }
    else if(PM25i<=150){
      a="%e5%b0%8d%e6%95%8f%e6%84%9f%e6%97%8f%e7%be%a4%e4%b8%8d%e5%81%a5%e5%ba%b7";
      b="對敏感族群不健康";
    }
    else if(PM25i<=200){
      a="%e5%b0%8d%e6%89%80%e6%9c%89%e6%97%8f%e7%be%a4%e4%b8%8d%e5%81%a5%e5%ba%b7";
      b="對所有族群不健康";
    }
    else if(PM25i<=300){
      a="%e9%9d%9e%e5%b8%b8%e4%b8%8d%e5%81%a5%e5%ba%b7";
      b="非常不健康";
    }
    else {
      a="%e5%8d%b1%e5%ae%b3%e5%8d%b1%e5%ae%b3";
      b="危害";
    }
    Serial.print("對健康影響: ");Serial.print(b);
    Serial.println("--------------------"); 

    String value1 = String(PM25);
    String value2 = a;
    Serial.print(value2);
    //String value3 = String(humidity);

     SendLineNotify(value1, value2);  //Client傳送資料
    
    delay(3000); // the OWM free plan API does NOT allow more then 60 calls per minute
}

void get(){
  HTTPClient http;
 
    http.begin("https://data.epa.gov.tw/api/v1/aqx_p_02?limit=1000&api_key=9be7b239-557b-4c10-9775-78cadfc555e9&sort=ImportDate%20desc&format=json", ca_cert);
    int httpCode = http.GET();
 
    if (httpCode > 0) {
 
        json=http.getString();
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end();
}






//char* GetWeatherData(){
//  
//  WiFiClient client;  //建立Client物件
//  const int httpPort = 80;  //預設通訊阜80
//  String JsonString = "";  //此範例不會用到
//
//  //Client連結Server
//  if (client.connect(host, httpPort)) {
//
//    //Client傳送
//    client.println(String("POST ") + resource + " HTTP/1.1");
//    client.println(String("Host: ") + host); 
//    client.println("Content-Type: application/json");
//    client.print("Content-Length: ");
//    client.println(JsonString.length());
//    client.println();
//    client.println(JsonString);
//
//    //等待5秒，每0.1秒偵測一次是否有接收到response資料  
//    int timeout = 0;
//    while(!client.available() && (timeout++ <= 50)){
//      delay(100);
//    }
//  
//    //如果無回應
//    if(!client.available()) {
//      Serial.println("No response...");
//    }
//
//    //Checking for the end of HTTP stream
//    while(!client.find("\r\n\r\n")){
//      // wait for finishing header stream reading ...
//    }
//
//    //讀取資料並儲存在jsonRead中
//    client.readBytes(jsonRead, 600);
//
//    //停止Client
//    client.stop(); 
//
//    //回傳
//    return jsonRead;
//  } 
//}

void SendLineNotify(String value1, String value2){
  
  WiFiClient client;  //建立Client物件
  const int httpPort = 80;  //預設通訊阜80
  String url = "";  //API url
  String JsonString = "";  //此範例不會用到

  //Client連結Server
  if (client.connect(host2, httpPort)) {
    
    //Webhook API
    url += "/trigger/" + String(event) + "/with/key/" + String(apiKey);
    //Query String
    url += "?value1=" + value1 + "&value2=" + value2;
Serial.println(url);
    //Client傳送
    client.println(String("POST ") + url + " HTTP/1.1");
    client.println(String("Host: ") + host2); 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(JsonString.length());
    client.println();
    client.println(JsonString);

    //等待5秒，每0.1秒偵測一次是否有接收到response資料  
    int timeout = 0;
    while(!client.available() && (timeout++ <= 50)){
      delay(100);
    }
  
    //如果無回應
    if(!client.available()) {
      Serial.println("No response...");
    }
    //用while迴圈一字一字讀取Response
    while(client.available()){
      Serial.write(client.read());
    }

    //停止Client
    client.stop(); 
  } 
}

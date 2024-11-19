# richardatomiccompany-1.2.0-XAMPPed
Versión estable de la página para controlar el RMVS-100 y su código para ESP8266 + ESP32CAM

Instrucciones para el correcto funcionamiento:
  1) Instale XAMPP y en la carpeta htdocs cree una carpeta con el nombre "richardatomiccompany"
  2) Proceda a definir su red Wi-Fi
  3) Encienda XAMPP activando Apache y PhpMyAdmin
  4) En su navegador predeterminado ingrese a "localhost/" y vaya al apartado de PhpMyAdmin
  5) Dentro de ahí, irá al apartado de SQL, donde copiará y pegará los archivos control_status.sql + dht22.sql (que se encuentra en la carpeta SQL)
  6) Una vez realizada la petición, vaya a "localhost/richardatomiccompany" y compruebe el correcto funcionamiento de la página
  7) En su editor de códigos, abra la carpeta "PROM"
  8) Edite la dirección IPv4 y agregue la de su red WiFi, al igual que su SSID y contraseña al ESP8266, y ESP32CAM
  9) Suba el código, y ya puede utilizar el vehículo

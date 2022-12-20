#include <main.h>
#include <Adafruit_FT6206.h>
#include <TFT_eSPI.h> // Include the graphics library (this includes the sprite functions)
#include <WiFi.h>

#define SSID "Starbucks WiFi"
#define PSW "idontknow"

TFT_eSPI tft = TFT_eSPI();              // Create object "tft"
Adafruit_FT6206 ts = Adafruit_FT6206(); // Touch control input
TFT_eSprite img = TFT_eSprite(&tft);    // Create Sprite object "img" with pointer to "tft" object

WiFiServer server(80);
String header;
//                                    // the pointer is used by pushSprite() to push it onto the TFT

void drawGrid()
{
  for (int i = 0; i < 12; i++)
  {
    tft.drawFastHLine(0, map(i, 0, 12, 0, tft.height()), tft.height(), TFT_WHITE);
    tft.drawFastVLine(map(i, 0, 12, 0, tft.height()), 0, tft.height(), TFT_WHITE);
  }

  tft.drawFastHLine(0, tft.height() - 1, tft.height(), TFT_WHITE);
  tft.drawFastVLine(tft.height() - 1, 0, tft.height(), TFT_WHITE);
}

int currentRed = 125;
int currentGreen = 125;
int currentBlue = 125;

int sliderHeight, pickerWidth, colorPatchHeight;

void drawPicker()
{
  int rBar = map(currentRed, 0, 255, 0, sliderHeight);
  int gBar = map(currentGreen, 0, 255, 0, sliderHeight);
  int bBar = map(currentBlue, 0, 255, 0, sliderHeight);

  for (int i = 0; i < sliderHeight; i++)
  {
    int c = map(i, 0, sliderHeight, 0, 255);
    int p = tft.height() - i;

    tft.drawFastHLine(240, p, pickerWidth, (rBar == i - 2 || rBar == i + 2) ? TFT_WHITE : tft.color565(c, currentGreen, currentBlue));
    tft.drawFastHLine(240 + pickerWidth, p, pickerWidth, (gBar == i - 2 || gBar == i + 2) ? TFT_WHITE : tft.color565(currentRed, c, currentBlue));
    tft.drawFastHLine(240 + 2 * pickerWidth, p, pickerWidth, (bBar == i - 2 || bBar == i + 2) ? TFT_WHITE : tft.color565(currentRed, currentGreen, c));
  }

  tft.fillRect(240, 0, pickerWidth * 3, colorPatchHeight, tft.color565(currentRed, currentGreen, currentBlue));
  tft.drawRect(240, 0, pickerWidth * 3, colorPatchHeight, TFT_BLACK);

  tft.setTextSize(3);
  tft.fillRect(240 + 2 * pickerWidth, 0, pickerWidth - 4, pickerWidth - 3, TFT_BLACK);
  tft.setCursor(241 + 2 * pickerWidth, 1);
  tft.print('X');
}

// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.print(SSID);
  WiFi.begin(SSID, PSW);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Done!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  tft.init();
  tft.setRotation(1);

  ts.begin();

  tft.fillScreen(TFT_BLACK);

  pickerWidth = (tft.width() - 240) / 3;
  colorPatchHeight = 40;
  sliderHeight = tft.height() - colorPatchHeight;

  drawGrid();
  drawPicker();
}

// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------
void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("New client connected");
    String currentLine = "";
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n' && currentLine.length() == 0)
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
        }
      }
    }
  }

  // Wait for a touch
  if (!ts.touched())
  {
    return;
  }

  // Retrieve a point
  TS_Point p = ts.getPoint();
  p.x = map(p.x, 0, 240, 240, 0);
  p.y = map(p.y, 0, 320, 320, 0);
  Serial.print("(");
  Serial.print(p.x);
  Serial.print(", ");
  Serial.print(p.y);
  Serial.println(")");

  // while (1)
  // {
  //   // Create the sprite and clear background to black
  //   img.createSprite(IWIDTH, IHEIGHT);
  //   // img.fillSprite(TFT_BLACK); // Optional here as we fill the sprite later anyway

  //   for (int pos = IWIDTH; pos > 0; pos--)
  //   {
  //     build_banner("Hello World", pos);
  //     img.pushSprite(0, 0);

  //     build_banner("TFT_eSPI sprite", pos);
  //     img.pushSprite(0, 50);

  //     delay(WAIT);
  //   }

  //   // Delete sprite to free up the memory
  //   img.deleteSprite();

  //   // Create a sprite of a different size
  //   numberBox(random(100), 60, 100);
  // }
}

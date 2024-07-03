#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RX_PIN 4         // Pino de recepção
#define TX_PIN 2         // Pino de transmissão
#define BUTTON_PIN_UP 26 // Pino do botão UP no encoder
#define BUTTON_PIN_DIR 27 // Pino do botão DIR no encoder
#define BUTTON_PIN_ESQ 12 // Pino do botão ESQ no encoder
#define BUTTON_PIN_ENC_BUTTON 0 // Pino do botão do encoder
#define FREQUENCY_SWITCH_PIN 13 // Pino do interruptor para mudar a frequência

#define OLED_RESET 22    // Pino de reset do OLED
#define SCREEN_WIDTH 128 // Largura da tela OLED
#define SCREEN_HEIGHT 32 // Altura da tela OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RCSwitch mySwitch = RCSwitch();

unsigned long receivedValue = 0; // Variável para armazenar o valor recebido
int receivedBitLength = 0;       // Comprimento do sinal recebido
int receivedProtocol = 0;        // Protocolo do sinal recebido
const int rssi_threshold = -75;  // Limiar de RSSI para considerar um sinal como forte o suficiente

static const uint32_t subghz_frequency_list[] = {
    /* Lista de frequências em MHz */
    300000000, 303875000, 304250000, 310000000, 315000000, 318000000,  // Faixa 300-348 MHz
    390000000, 418000000, 433075000, 433420000, 433920000, 434420000, 434775000, 438900000,  // Faixa 387-464 MHz
    868350000, 915000000, 925000000  // Faixa 779-928 MHz
};

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN_ENC_BUTTON, INPUT_PULLUP); // Botão do encoder como entrada com pull-up
  pinMode(BUTTON_PIN_UP, INPUT_PULLUP); // Botão UP do encoder como entrada com pull-up
  pinMode(BUTTON_PIN_DIR, INPUT_PULLUP); // Botão DIR do encoder como entrada com pull-up
  pinMode(BUTTON_PIN_ESQ, INPUT_PULLUP); // Botão ESQ do encoder como entrada com pull-up
  pinMode(FREQUENCY_SWITCH_PIN, INPUT_PULLUP); // Pino do interruptor como entrada com pull-up

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Inicializa o display OLED
    Serial.println(F("Falha ao iniciar o display OLED"));
    while (1); // Fica preso aqui se houver falha no display
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("      |--------|");
  display.println("      |Scary-RF|");
  display.println("      |  TOOL  |");
  display.println("      |________|");
  display.display();
  delay(2000);
  display.clearDisplay();

  ELECHOUSE_cc1101.Init(); // Inicializa o módulo CC1101

  ELECHOUSE_cc1101.setMHZ(433.92); // Configura a frequência inicial
  ELECHOUSE_cc1101.SetRx(); // Configura o módulo CC1101 para receber

  mySwitch.enableReceive(RX_PIN); // Habilita a recepção no pino RX
  mySwitch.enableTransmit(TX_PIN); // Habilita a transmissão no pino TX
}

void loop() {
  if (mySwitch.available()) { // Se houver dados disponíveis para leitura
    receivedValue = mySwitch.getReceivedValue(); // Lê o valor recebido
    receivedBitLength = mySwitch.getReceivedBitlength(); // Obtém o comprimento do sinal recebido
    receivedProtocol = mySwitch.getReceivedProtocol(); // Obtém o protocolo do sinal recebido

    if (receivedValue != 0) { // Se o valor recebido for diferente de 0
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Received Signal:");
      display.setCursor(0, 10);
      display.println(receivedValue);
      display.setCursor(0, 20);
      display.printf("Bit:%d", receivedBitLength);
      display.printf("  Ptc:%d", receivedProtocol);
      display.display();

      mySwitch.resetAvailable(); // Reinicia o buffer de dados recebidos
    }
  }

  // Lógica para botão do encoder
  if (digitalRead(BUTTON_PIN_ENC_BUTTON) == LOW) {
    // Ação ao pressionar o botão do encoder
    // Por exemplo, você pode adicionar aqui uma ação específica
    delay(200); // Debounce
  }

  // Lógica para botão UP do encoder
  if (digitalRead(BUTTON_PIN_UP) == LOW) {
    // Ação ao pressionar o botão UP do encoder
    // Por exemplo, você pode adicionar aqui uma ação específica
    delay(200); // Debounce
  }

  // Lógica para botão DIR do encoder
  if (digitalRead(BUTTON_PIN_DIR) == LOW) {
    // Ação ao pressionar o botão DIR do encoder
    // Por exemplo, você pode adicionar aqui uma ação específica
    delay(200); // Debounce
  }

  // Lógica para botão ESQ do encoder
  if (digitalRead(BUTTON_PIN_ESQ) == LOW) {
    // Ação ao pressionar o botão ESQ do encoder
    // Por exemplo, você pode adicionar aqui uma ação específica
    delay(200); // Debounce
  }

  // Lógica para mudança de frequência
  if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
    ELECHOUSE_cc1101.setMHZ(315); // Muda a frequência para 315MHz
  } else {
    ELECHOUSE_cc1101.setMHZ(433.92); // Mantém a frequência em 433.92MHz
  }

  // Lógica para enviar um sinal aleatório
  if (digitalRead(BUTTON_PIN_DIR) == LOW) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sending Random:");
    display.display();

    delay(100);

    // Gera uma sequência aleatória de 9 dígitos
    unsigned long randomValue = 100000000 + random(900000000);

    int randomBitLength = 28; // Ajuste conforme necessário
    int randomProtocol = 6;   // Ajuste conforme necessário

    if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
      // Configura o protocolo e comprimento do sinal para frequência alternativa
      randomBitLength = 24; // Ajuste conforme necessário
      randomProtocol = 1;   // Ajuste conforme necessário
    }

    mySwitch.disableReceive();
    delay(100);
    mySwitch.enableTransmit(TX_PIN);
    ELECHOUSE_cc1101.SetTx();

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sending Random:");
    display.setCursor(0, 10);
    display.println(randomValue);
    display.setCursor(0, 20);
    display.println("Sending...");
    display.display();

    mySwitch.setProtocol(randomProtocol);
    mySwitch.send(randomValue, randomBitLength);

    delay(100);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sending Random:");
    display.setCursor(0, 10);
    display.println(randomValue);
    display.setCursor(0, 20);
    display.println("OK");
    display.display();

    ELECHOUSE_cc1101.SetRx();
    mySwitch.disableTransmit();
    delay(100);
    mySwitch.enableReceive(RX_PIN);
  }

  // Lógica para análise de frequência e potência
  if (digitalRead(BUTTON_PIN_UP) == LOW) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Frequency Analysis:");
    display.display();

    ELECHOUSE_cc1101.SetRx();
    delay(500);

    for (int i = 0; i < sizeof(subghz_frequency_list) / sizeof(subghz_frequency_list[0]); i++) {
      ELECHOUSE_cc1101.setMHZ(subghz_frequency_list[i] / 1000000);
      delay(100);

      int rssi = ELECHOUSE_cc1101.getRssi();
      Serial.print("RSSI: ");
      Serial.println(rssi);

      if (rssi >= rssi_threshold) {
        display.setCursor(0, 10);
        display.println(subghz_frequency_list[i]);
        display.setCursor(0, 20);
        display.println("Found");
        display.display();
      } else {
        display.setCursor(0, 20);
        display.println(" ");
        display.display();
      }
    }
  }
}

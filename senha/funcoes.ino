// --------------------------------------------------- Configuração
void configurarPinos() {
  for (byte i = 0; i < sizeof(leds); i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  for (byte i = 0; i < sizeof(bots); i++) {
    pinMode(bots[i], INPUT_PULLUP);
  }
}

void configurarComunicarPC() {
  Serial.begin(9600);
}

//void novoAleatorio() {
  
//}

// ---------------------------------------------------- LEITURAS

boolean botao(byte b) { // retorna o estado do botão
  return !digitalRead(b);
}

boolean clique(byte b) { // retorna TRUE no momento em que o LED é pressionado
  static unsigned long timer[sizeof(bots)];
  static long lastR = 0;
  boolean r = botao(b);
  b = qualBotao(b);

  if (millis() - timer[b] > 10) {
    if (r != bitRead(lastR, b)) {
      bitWrite(lastR, b, r);
      if (r) {
        return true;
      }
    }
    timer[b] = millis();
  }
  return false;
}

boolean qualquerClique() {
  if (clique(BOT_1) || clique(BOT_2) || clique(BOT_3)) {
    return true;
  }
  return false;
}

// ------------------------- LUZ

void acende(byte led) {
  digitalWrite(led, HIGH);
}

void apaga(byte led) {
  digitalWrite(led, LOW);
}

void atualizarLuz() {
  for (byte i = 0; i < sizeof(leds); i++) {
    digitalWrite(leds[i], aceso[i]);
  }
}

void mudarLuz(byte led) { // muda o estado do LED
  led = qualLed(led);
  aceso[led] = !aceso[led];
}

void pisca(byte vezes, uint16_t velocidade) { // LED específico (interrompendo todas as outras rotinas)
  byte conta = 0;

  while (conta < vezes) {
    for (byte i = 0; i < sizeof(leds); i++) {
      acende(leds[i]);
    }
    delay(velocidade);

    for (byte i = 0; i < sizeof(leds); i++) {
      apaga(leds[i]);
    }
    delay(velocidade);
    conta++;
  }
}

// ------------------ CHECAGEM

byte qualBotao(byte botao) {
  for (byte i = 0; i < sizeof(bots); i++) {
    if (botao == bots[i]) {
      return i;
    }
  }
}

byte qualLed(byte led) {
  for (byte i = 0; i < sizeof(leds); i++) {
    if (led == leds[i]) {
      return i;
    }
  }
}

void checarTentativa() {
  byte digito = 0;
  boolean senhaLida[sizeof(senha)] = {false, false, false}, tentativaLida[sizeof(senha)] = {false, false, false};

  for (byte i = 0; i < sizeof(estados); i++) {
    estados[i] = 0;
  }

  if(primeiraVez){
    while(tentativa[0] == senha[0] && tentativa[1] == senha[1] && tentativa[2] == senha[2]){
      gerarSenha();
    }
    primeiraVez = false;
  }

  for (byte i = 0; i < sizeof(senha); i++) {
    if (tentativa[i] == senha[i]) {
      estados[digito] = 2;
      senhaLida[i] = true;
      tentativaLida[i] = true;
      digito++;
      if (teste) Serial.println("E2: i = " + String(i));
    }
  }

  for (byte i = 0; i < sizeof(tentativa); i++) {
    if (tentativaLida[i]) {
      if (teste) Serial.println("debug 1");
      continue;
    }
    for (byte j = 0; j < sizeof(senha); j++) {
      if (senhaLida[j] || tentativaLida[i]) {
        if (teste) Serial.println("debug 2");
        continue;
      } else if (tentativa[i] == senha[j]) {
        estados[digito] = 1;
        digito++;
        tentativaLida[i] = true;
        senhaLida[j] = true;
        if (teste) Serial.println("E1: i = " + String(i) + ", j = " + String(j));
      }
    }
  }
  if (teste) {
    Serial.print("S: ");
    for (byte i = 0; i < sizeof(senha); i++) {
      Serial.print(senha[i]);
    }
    Serial.println();
    Serial.print("T: ");
    for (byte i = 0; i < sizeof(tentativa); i++) {
      Serial.print(tentativa[i]);
    }
    Serial.println();
    Serial.print("E: ");
    for (byte i = 0; i < sizeof(estados); i++) {
      Serial.print(estados[i]);
    }
    Serial.println();
  }
}

void resultado() {
  static unsigned long timer = 0;
  byte conta = 0;
  
  for(byte i = 0; i < sizeof(estados); i++){
    if(estados[i] == 2){
      conta++;
    }
  }
  if(conta == 3){
    pisca(10, 50);
    //gerarSenha();
    contador = 0;
    primeiraVez = true;
  } else if (millis() - timer > 500) {
    for (byte i = 0; i < sizeof(estados); i++) {
      switch (estados[i]) {
        case 0:
          aceso[i] = false;
          break;
        case 1:
          mudarLuz(leds[i]);
          break;
        case 2:
          aceso[i] = true;
          break;
      }
      atualizarLuz();
    }
    timer = millis();
  }
}

// --------------------------- JOGO

int aleatorio(int m, int M) {
  randomSeed(analogRead(0));
  return random(m, M);
}

int aleatorio(int M) {
  randomSeed(analogRead(0));
  return random(M);
}

void gerarSenha() {
  for (byte i = 0; i < sizeof(senha); i++) {
    senha[i] = aleatorio(sizeof(bots));
  }
}

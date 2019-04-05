/* Caio Kenji, 20/02/2019
    Jogo cooperativo simples de senha para duas pessoas
*/

#define BOT_1 2
#define BOT_2 3
#define BOT_3 4

#define LED_1 5
#define LED_2 6
#define LED_3 7

byte bots[] = {BOT_1, BOT_2, BOT_3}, leds[] = {LED_1, LED_2, LED_3}; // Coloca os leds e botões em uma lista para acesso rápido
byte senha[sizeof(leds)], tentativa[sizeof(leds)]; // variáveis para guardar a senha e as tentativas para descobri-la
byte estados[sizeof(senha)]; // guarda nesta ordem: quantos cliques certos na ordem certa (cod. 2), quantos cliques certos na ordem errada (cod. 1) e cliques errados (cod. 0)
byte contador = 0, ultimoC = 0; // quantos cliques durante a tentativa
boolean aceso[sizeof(leds)] = {false, false, false}; // se os leds devem estar acesos ou não
boolean primeiraVez = true; // se é a primeira tentativa
boolean terminou = false; // se 3 cliques foram dados
boolean teste = true; // se está em modo teste

void setup() {
  configurarPinos(); // configura pinos
  configurarComunicarPC(); // configura comunicação
  pisca(5, 50); // pisca para iniciar
  if(teste) Serial.println("MODO TESTE");
}

void loop() {
  if (contador < 3) { // se não tiver apertado 3x qualquer botão
    if (aceso[0] || aceso[1] || aceso[2]) { // se alguma luz indicar para estar acesa
      aceso[0] = false;
      aceso[1] = false;
      aceso[2] = false;
      atualizarLuz(); // apagar todas
    }
    for (byte i = 0; i < sizeof(bots); i++) { // para cada botão
      if (clique(bots[i])) { // verificacr se foi apertado
        tentativa[contador] = i;
        contador++;
        terminou = true;
      }
    }
  } else {
    for (byte i = 0; i < sizeof(bots); i++) {
      if (clique(bots[i])) {
        tentativa[0] = i;
        contador = 1;
      }
    }
    if (terminou) {
      checarTentativa();
      terminou = false;
    }
    resultado();
  }
  if (teste && contador != ultimoC) {
    Serial.println("Contador = " + String(contador));
    ultimoC = contador;
  }
}

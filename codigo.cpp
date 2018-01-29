#include <Wire.h>
#include <LiquidCrystal.h>

#define atualizaTela 80 // Tempo definido para atualizar a tela

#define DS1307_ADDRESS 0x68 // Define o pino do RTC
#define botaoTela 2 // Botao de troca de tela
#define botaoEntra 5 // Botao enter
#define botaoSeta 6 // Botao de seleção
#define buzzer 7 // Pino do buzzer

//Canteiro 1
#define us1 1 // pino sensores de umidade do solo
#define us2 2
// Canteiro 2
#define us3 3
#define us4 4
//Canteiro 3
#define us5 5
#define us6 6
// Sensores
#define lm35 0; //pino do sensor de temperatura
#define ldr 7; // pino sensor de luminosidade

byte zero = 0x00;
int rtc_horas, rtc_mes, rtc_diadomes;
int bd_culturas[33][12] = {{0, 22, 50, 70, 85, 20, 7, 0, 600, -1, -1, -1}, // Default
                          {1, 20, 27, 40, 60, 10, 12, 900, 600, 45, 25, 4}, // Abobrinha
                          {2, 16, 18, 35, 45, 7, 10, 850, 650, 70, 30, 5}, // Acelga
                          {3, 10, 20, 40, 60, 5, 11, 950, 600, 60, 40,6}, // Agrião
                          {4, 13, 18, 35, 45, 7, 9, 900, 750, 4, -1, -1}, //Alcachofra
                          {5, 20, 25, 55, 65, 10, 12, 800, 600, 50, 7, -1}, //Alface
                          {6, 13, 24, 40, 60, 7, 10, 750, 600, 120, -1, 6}, //Alho poró
                          {7, 15, 20, 40, 60, 7, 10, 800, 600, 60, 60, 7}, //Batata
                          {8, 18, 25, 50, 60, 10, 12, 900, 750, 90, 20, 3}, //Berinjela
                          {9, 10, 20, 35, 45, 7, 10, 900, 750, 60, 25,5}, //Beterraba
                          {10, 18, 23, 50, 60, 8, 11, 850, 600, 100, 35, 6}, //Brocólis
                          {11, 13, 25, 40, 60, 6, 10, 750, 600, 150, 40, 4}, //Cebola
                          {12, 8, 20, 40, 60, 6, 10, 800, 600, 70, 50, 5}, //Cenoura
                          {13, 9, 19, 40, 60, 6, 10, 800, 600, 60, 30, 6}, //Couve
                          {14, 13, 25, 50, 65, 7, 10, 900, 750, 60, 30, 4}, //Couve-flor
                          {15, 13, 18, 50, 65, 7, 10, 800, 600, 75, 40, 7}, //Ervilha
                          {16, 13, 20, 50, 65, 7, 10, 750, 600, 80, 45, 3}, //Espinafre
                          {17, 9, 11, 40, 60, 6, 10, 900, 750, 110, 50, 2}, //Grão de bico
                          {18, 20, 25, 55, 65, 10, 12, 750, 600, 180, 70, 4}, //Inhame
                          {19, 15, 18, 45, 65, 9, 12, 800, 600, 90, 20, 5}, //Mandioquinha
                          {20, 15, 27, 55, 65, 9, 12, 800, 600, 65, 35, 6}, //Mostarda
                          {21, 14, 22, 40, 60, 6, 10, 850, 650, 70, 50, 4}, //Nabo
                          {22, 18, 30, 55, 65, 8, 12, 750, 650, 50, 65, 5}, //Pepino
                          {23, 21, 27, 60, 65, 8, 12, 750, 600, 110, 30, 3}, //Pimentão
                          {24, 15, 27, 40, 60, 7, 10, 900, 750, 75, 25, 7}, //Quiabo
                          {25, 17, 25, 50, 60, 8, 12, 750, 600, 120, 40, 2}, //Repolho
                          {26, 25, 28, 55, 65, 10, 12, 750, 600, 75, 45, 4}, //Taioba
                          {27, 15, 19, 40, 60, 8, 11, 900, 700, 100, 60, 5}, //Tomates
                          {28, 20, 25, 50, 60, 9, 12, 750, 600, 70, 30, 3}, //Vagem
                          {29, 22, 50, 70, 85, 20, 7, 0, 600, 3, -1, -1}, // Manjericão
                          {30, 23, 50, 71, 86, 20, 7, 0, 600, 3, -1, -1}, // Orégano
                          {31, 24, 50, 72, 87, 20, 7, 0, 600, 3, -1, -1}, // Tomilho
                          {32, 25, 50, 73, 88, 20, 7, 0, 600, 3, -1, -1}}; // Alecrim

LiquidCrystal lcd(13, 12, 8, 9, 10, 11); // Define pinos de dados do LCD

/*int us1 = 1; //pino sensores de umidade do solo
int us2 = 2;
int us3 = 3;
int us4 = 4;
int us5 = 5;
int us6 = 6;
int uc1, uc2, uc3;
int ldr = 7;
const int botaoTela = 2;
const int botaoEntra = 5;
const int botaoSeta = 6;
float um[3];
int buzzer = 7; // pino do buzzer*/

int rele[3] = {50, 49, 48};
int notif[3][3];
int lumin;

int um1; //variaveis que recebem valor dos sensores de umidade
int um2;
int um3;
int um4;
int um5;
int um6;
float temp; // Variavel que recebe a leitura do LM35
int statusCanteiro[3][2]; // matriz que armazena a cultura em cada canteiro

//Menu
int Tela = 0; // Variavel que vai trabalhar na seleçao de Telas do LCD
int bconta_clicks = 0; // Variavel que conta o numero de clicks dado nos botoes
int bstatus_anterior = 0; // Participa da comparação referente ao estado do botaoTela
int bstatus_atual = 0; // Recebe a leitura do botaoTela

int click_selCanteiro = 1; // Variavel que vai receber o numero do canteiro a ser selecionado das matrizes
int click_selCultura = 1; // Variavel que vai receber o numero da cultura a ser selecionada das matrizes
boolean inib; // Variavel booleana para mudança de estado da telaSetup
int contRTC[3]; // Variavel que faz a comparação dos tempos de colheita, germinação e preparo de cada cultura

void setup()
{
  //attachInterrupt(leDados, 10000);
  lcd.begin(16,2); // Seta o display LCD 16x2
  Wire.begin(); // Inicia a comunicação I2C
  pinMode(buzzer, OUTPUT); // Define o pino buzzer como saída
  pinMode(botaoTela, INPUT); // Define o botaoTela como entrada
  pinMode(lm35,INPUT); // Define o pino lm35 como entrada
  pinMode(rele[0], OUTPUT); // Define os pinos rele[i] tal que i vai de 0 a 2 como saída
  pinMode(rele[1], OUTPUT);
  pinMode(rele[2], OUTPUT);
  pinMode(us1,INPUT); // Define os pinos usx, sendo x de 1 a 6 como entrada
  pinMode(us2,INPUT);
  pinMode(us3,INPUT);
  pinMode(us4,INPUT);
  pinMode(us5,INPUT);
  pinMode(us6,INPUT);
  Serial.begin(9600); // Inicia a comunicação serial com a taxa de transferencia de 9600 baud

  statusCanteiro[0][0] = {1}; // Define a primeira coluna da matriz statusCanteiro como 1 2 e 3, ID de cada canteiro.
  statusCanteiro[1][0] = {2};
  statusCanteiro[2][0] = {3};


  byte graus[8] = { B01110, B01010, B01010,  B001110, B00000, B00000, B00000, B00000};   // Criando caractere graus, para mostrar a temperatura no display
  lcd.createChar(1, graus);   // Define  º como uma variável
}

void loop()
{
  leDados(); // Chama funçao que engloba a leitura dos três sensores principais
  verifNotificacao(); // Chama a funçao que verifica se existe alguma notificacao pendente

  bstatus_atual = digitalRead(botaoTela); // Lê o estado do botaoTela e armazena em bstatus_atual
  if (bstatus_atual != bstatus_anterior){ // Compara se o estado do botaoTela e diferente do anterior
    if (bstatus_atual == HIGH){ // Se o bstatus_atual (estado de botaoTela) e alto...
      delay (250); // Aguarda 250ms
      bconta_clicks++; // Incrementa bconta_clicks em 1 unidade
      Tela = bconta_clicks; // Tela recebe o numero de clicks que o botaoTela recebeu
    }
    if (bconta_clicks > 1){ // Como so existem 2 telas, se o valor de clicks passar de 1, volta para 0
      bconta_clicks = 0;
    }
  }
  bstatus_anterior = bstatus_atual;
  switch (Tela) { // Inicia o Switch case.
      default: {
        telaInicial(); // Chama a funçao que define a telaInicial
        delay(atualizaTela);
        break;
      }

      case 1: {
        telaSetup(); // Chama a funçao que define a telaSetup, tela que configura os canteiros e suas culturas
        delay(atualizaTela);
        break;
      }
  }
  delay(100);
}


void leDados(){
  lerTemp(); // Chama a funçao de leitura de temperatura
  lerUmid(); // Chama a funçao de leitura da umidade do solo
  lerLum(); // Chama a funçao de leitura da luminosidade
}
void lerTemp(){
  temp = 0.5*(float(analogRead(lm35))*5/(1023))/0.01; // Lê e trata o dado do LM35 (sensor de temperatura)
  int vetor[100]; // Define um vetor int de 100 posições
  int soma, media;
  for(int i=0; i<100; i++){ // Joga dentro da posição i a leitura de 100 temperaturas, a cada 50ms, em seguida, faz a media e joga na variavel temp a media desses valores.
    vetor[i] = temp;
    soma += vetor[i];
    delay(50);
  }
  media = soma/100;
  temp = media;
}

void lerLum(){
  lumin = analogRead(ldr); // Leitura do LDR, 0 a 1023
}

void telaIrrig(){ // Tela que comunica ao usuario que está ocorrendo a irrigação e ele deve esperar acabar até fazer alguma configuraçao ou consulta
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Irrigando");
  lcd.setCursor(2,1);
  lcd.print("Aguarde...");
}

void lerUmid(){ // Função que lê os dados dos 6 sensores de umidade do solo, converte para porcentagem e faz a media dos sensores de umidade de cada canteiro
  um1 = analogRead(us1);
  um2 = analogRead(us2);
  um3 = analogRead(us3);
  um4 = analogRead(us4);
  um5 = analogRead(us5);
  um6 = analogRead(us6);
  um1 = map(um1, 1023, 555, 0, 100);
  um2 = map(um2, 1023, 555, 0, 100);
  um3 = map(um3, 1023, 555, 0, 100);
  um4 = map(um4, 1023, 555, 0, 100);
  um5 = map(um5, 1023, 555, 0, 100);
  um6 = map(um6, 1023, 555, 0, 100);
  uc1 = (um1+um2)*0.5;
  uc2 = (um3+um4)*0.5;
  uc3 = (um5+um6)*0.5;
  um[0] = uc1;
  um[1] = uc2;
  um[2] = uc3;
  //delay(1000);
}

void telaInicial(){
  lcd.clear();
  lcd.setCursor(1,0);
  Mostrarelogio(); // Imprime a hora, minuto e data vindos do RTC
  lcd.setCursor(0,1);
  lcd.print("T ");
  lcd.print(temp, 1); lcd.write(1); lcd.print("C ");
  lcd.print("L ");
  lcd.print(lumin);
  acionamentoSistema(); // Chama a funçao que aciona as valvulas solenoides para irrigaçao
}

void verifNotificacao(){
  if(millis() % 4000 <= 150 && millis() % 4000 >= 0){ // Faz um contador a cada 4000ms para definir a notificacao de colheita, preparo ou germinaçao da cultura, no caso foi feito de 4 segundos para apresentaçao, o correto seria 86400000ms, equivalente a 24 horas.
    for(int i=0; i<3; i++){
      contRTC[i]++;
    }
  }
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(notif[i][j] == contRTC[i]){ // Compara se a matriz que possui os parametros de colheita, plantio e germinaçao de cada canteiro, baseado na cultura definida na telaSetup
        if(j == 0){ // Se a condiçao anterior foi satisfeita e j == 0 significa que deu o tempo de realizar a colheita
          while(inib == 0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Colher cant. "); lcd.print(i+1); lcd.print(".");
            Serial.print("Colher canteiro "); Serial.println(i+1);
            if(digitalRead(botaoEntra) == 1){
              inib = !inib;
              delay(400);
            }
            Tela = 3;
          }
          //inib = 0;
        }
        else if(j == 1){ // Se a condiçao anterior foi satisfeita e j == 1 significa que deu o tempo de germinação
          while(inib == 0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Preparar cant. "); lcd.print(i+1); lcd.print(".");
            Serial.print("Preparar canteiro "); Serial.println(i+1);
            if(digitalRead(botaoEntra) == 1){
              inib = !inib;
              delay(400);
            }
            Tela = 3;
          }
          //inib = 0;
        }
        else if(j == 2){ // Se a condiçao anterior foi satisfeita e j == 2 significa que deu o tempo para preparo da terra
          while(inib == 0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Semear cant. "); lcd.print(i+1); lcd.print(".");
            Serial.print("Semear canteiro "); Serial.println(i+1);
            if(digitalRead(botaoEntra) == 1){
              inib = !inib;
              delay(400);
            }
            Tela = 3; // Volta para a tela default (telaInicial)
          }
        }
      }
    }
  }
}

void telaSetup(){ // Tela em que e feita a definiçao dos canteiros e suas respectivas culturas
  lcd.clear(); // limpa o display lcd
  inib = 0;
  while(inib == 0){
    lcd.setCursor(0,0);
    lcd.print("Sel. canteiro: ");
    lcd.print(click_selCanteiro);
    lcd.setCursor(0,1);
    lcd.print("                ");
    if(digitalRead(botaoSeta) == 1){
      click_selCanteiro++;
      delay(600);
    }
    if(click_selCanteiro == 4){
      click_selCanteiro = 1;
    }
    if(digitalRead(botaoEntra) == 1){
      inib = !inib;
      delay(600);
    }
  }

  while(inib == 1){
    lcd.setCursor(0,1);
    lcd.print("Sel. cultura: ");
    if(click_selCultura < 10){
      lcd.print("0");
    }
    lcd.print(click_selCultura);
    if(digitalRead(botaoSeta) == 1){
      click_selCultura++;
      delay(600);
    }
    if(click_selCultura == 34){
      click_selCultura = 1;
    }
    if(digitalRead(botaoEntra) == 1){
      statusCanteiro[click_selCanteiro - 1][1] = click_selCultura; // Define a cultura no canteiro de acordo com a interaçao com o usuario
      contRTC[click_selCanteiro - 1] = 0; // Zera o contador para aquela cultura
      for(int j=0; j<3; j++){
        notif[click_selCanteiro - 1][j] = bd_culturas[click_selCultura][9+j]; // Define os tempos de colheita, germinaçao e preparo no vetor notif[] para a cultura definida
      }
      inib = !inib;
    }
    Tela = 3;
  }
}

void acionamentoSistema(){
  int contadorBuzzer; // variavel local para assegurar que o buzzer so será acionado uma vez a cada valvula acionada
  for(int i=0; i<3; i++){
    digitalWrite(rele[i], HIGH); // Desliga todos os relés antes de iniciar o codigo
  }
  for(int i=0; i<3; i++){
    if(temp > bd_culturas[statusCanteiro[i][1]][1] && temp < bd_culturas[statusCanteiro[i][1]][2]){ // Garante que a temperatura estará na faixa adequada para a cultura, baseado na matriz de parametros
      Serial.println("temp ok");
      if(lumin > bd_culturas[statusCanteiro[i][1]][7] && lumin < bd_culturas[statusCanteiro[i][1]][8]){ // Garante que a luminosidade estará na faixa adequada para a cultura, baseado na matriz de parametros
        Serial.println("lumin ok");
        if((rtc_horas >= bd_culturas[statusCanteiro[i][1]][5]) || (rtc_horas <= bd_culturas[statusCanteiro[i][1]][6])){ // Garante que o horario estará na faixa adequada para a cultura, baseado na matriz de parametros
          Serial.println("horario ok");
          contadorBuzzer = 0;
          while(um[i] < (0.5*(bd_culturas[statusCanteiro[i][1]][3]+bd_culturas[statusCanteiro[i][1]][4]))){ // Garante que a umidade não está na faixa adequada para a cultura, baseado na matriz de parametros e, enquanto o solo nao estiver adequado, continua irrigando
            lerUmid();
            lerTemp();
            lerLum();
            telaIrrig();
            if(contadorBuzzer == 0){
              digitalWrite(buzzer, HIGH); delay(1500); // Aciona o alerta para o usuario, comunicando que algum canteiro será irrigado
              digitalWrite(buzzer, LOW);
              contadorBuzzer++;
              delay(3000); // Define um tempo de espera até que a valvula seja ligada, para que o usuario possa sair do ambiente a ser molhado, 3 segundos foi para a apresentaçao
            }
            digitalWrite(rele[i], LOW);
          }
          digitalWrite(rele[i], HIGH);
        }
      }
    }
  }
}

void SelecionaDataeHora()   //Seta a data e a hora do RTC, que armazena e mantem por conta da bateria. Nao se perde a contagem devido a queda ou desligamento do uC.
{
  byte segundos = 00; //Valores de 0 a 59
  byte minutos = 33; //Valores de 0 a 59
  byte horas = 00; //Valores de 0 a 23
  byte diadasemana = 1; //Valores de 0 a 6 - 0=Domingo, 1 = Segunda, etc.
  byte diadomes = 29; //Valores de 1 a 31
  byte mes = 1; //Valores de 1 a 12
  byte ano = 17; //Valores de 0 a 99
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //Stop no CI para que o mesmo possa receber os dados

  //As linhas abaixo escrevem no CI os valores de
  //data e hora que foram colocados nas variaveis acima
  Wire.write(ConverteParaBCD(segundos));
  Wire.write(ConverteParaBCD(minutos));
  Wire.write(ConverteParaBCD(horas));
  Wire.write(ConverteParaBCD(diadasemana));
  Wire.write(ConverteParaBCD(diadomes));
  Wire.write(ConverteParaBCD(mes));
  Wire.write(ConverteParaBCD(ano));
  Wire.write(zero); //Start no CI
  Wire.endTransmission();
}

byte ConverteParaBCD(byte val){ //Converte o número de decimal para BCD
  return ( (val/10*16) + (val%10) );
}

byte ConverteparaDecimal(byte val)  { //Converte de BCD para decimal
  return ( (val/16*10) + (val%16) );
}

void Mostrarelogio()
{
  Wire.beginTransmission(DS1307_ADDRESS); // Inicia a comunicaçao I2C com o endereço DS1307_ADDRESS
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int segundos = ConverteparaDecimal(Wire.read());
  int minutos = ConverteparaDecimal(Wire.read());
  int horas = ConverteparaDecimal(Wire.read() & 0b111111);
  rtc_horas = horas;
  int diadasemana = ConverteparaDecimal(Wire.read());
  int diadomes = ConverteparaDecimal(Wire.read());
  rtc_diadomes = diadomes;
  int mes = ConverteparaDecimal(Wire.read());
  rtc_mes;
  int ano = ConverteparaDecimal(Wire.read());
  //Mostra a data no Serial Monitor
  //lcd.print("Data: ");
  if(diadomes < 10) // Garante uma apresentacao estetica homogenea para a data.
    lcd.print("0");
  lcd.print(diadomes);
  lcd.print("/");
  if(mes < 10)
    lcd.print("0");
  lcd.print(mes);
  lcd.print("/");
  lcd.print(ano);
  lcd.print(" ");
  if(horas < 10)
    lcd.print("0");
  lcd.print(horas);
  lcd.print(":");
  if(minutos < 10)
    lcd.print("0");
  lcd.print(minutos);
}

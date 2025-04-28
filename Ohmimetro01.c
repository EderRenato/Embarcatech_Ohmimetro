#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define ADC_PIN 28 // GPIO para o voltímetro

char colors[10][10] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"};
int R_conhecido = 10000;   // Resistor de 10k ohm
float R_x = 0.0;           // Resistor desconhecido
float ADC_VREF = 3.30;     // Tensão de referência do ADC
int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)

void calcular_cores(float resistencia, char *cor1, char *cor2, char *cor3) {
  int primeira_casa, segunda_casa, multiplicador = 0;

  // Normaliza o valor para ficar entre 10 e 99
  while (resistencia >= 100) {
      resistencia /= 10;
      multiplicador++;
  }
  while (resistencia < 10 && resistencia > 0) {
      resistencia *= 10;
      multiplicador--;
  }

  int valor = (int)(resistencia + 0.5f); // Arredonda

  primeira_casa = valor / 10;
  segunda_casa = valor % 10;

  // Proteção caso arredonde pra 100
  if (primeira_casa == 10) {
      primeira_casa = 1;
      segunda_casa = 0;
      multiplicador++;
  }

  strcpy(cor1, colors[primeira_casa]);
  strcpy(cor2, colors[segunda_casa]);
  strcpy(cor3, colors[multiplicador]);
}

float corrigir_para_E24(float resistencia) {
  // Valores da série E24 (para uma década)
  float E24[] = {10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30,
                 33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91};
  int num_valores = sizeof(E24) / sizeof(E24[0]);

  // Calcula a década (potência de 10) mais próxima
  float decada = 1.0f;
  while (resistencia >= 100.0f) {
      resistencia /= 10.0f;
      decada *= 10.0f;
  }
  while (resistencia < 10.0f && resistencia > 0.0f) {
      resistencia *= 10.0f;
      decada /= 10.0f;
  }

  // Agora resistencia está entre 10 e 99

  // Procura o valor E24 mais próximo
  float melhor = E24[0];
  float menor_erro = fabs(resistencia - E24[0]);
  for (int i = 1; i < num_valores; i++) {
      float erro = fabs(resistencia - E24[i]);
      if (erro < menor_erro) {
          melhor = E24[i];
          menor_erro = erro;
      }
  }

  return melhor * decada;
}

int main()
{

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_t ssd;                                                // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  adc_init();
  adc_gpio_init(ADC_PIN); // GPIO 28 como entrada analógica

  float tensao;
  char str_x[5]; // Buffer para armazenar a string
  char str_y[5]; // Buffer para armazenar a string

  bool cor = true;
  while (true)
  {
    adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica

    float soma = 0.0f;
    for (int i = 0; i < 500; i++)
    {
      soma += adc_read();
      sleep_ms(1);
    }
    float media = soma / 500.0f;

      // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_RESOLUTION - adc_encontrado)
      R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

    sprintf(str_x, "%1.0f", media); // Converte o inteiro em string
    sprintf(str_y, "%1.0f", R_x);   // Converte o float em string

    char cor1[10], cor2[10], cor3[10];  // Variáveis para armazenar as cores
    float R_corrigido = corrigir_para_E24(R_x); // Corrige o valor para a série E24

    calcular_cores(R_corrigido, cor1, cor2, cor3);  // Calcula as cores das faixas 1, 2 e 3
    //  Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
    ssd1306_line(&ssd, 3, 15, 123, 15, cor);           // Desenha uma linha
    ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
    ssd1306_draw_string(&ssd, "  Ohmimetro", 8, 6);  // Desenha uma string
    ssd1306_draw_string(&ssd, "ADC", 13, 41);          // Desenha uma string
    ssd1306_draw_string(&ssd, "Resisten.", 50, 41);    // Desenha uma string
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);           // Desenha uma linha vertical
    ssd1306_draw_string(&ssd, cor1, 5, 18);        // Desenha a primeira faixa
    ssd1306_draw_string(&ssd, cor2, 75, 18);       // Desenha a segunda faixa
    ssd1306_draw_string(&ssd, cor3, 5, 27);        // Desenha a terceira faixa
    ssd1306_draw_string(&ssd, "Gold", 75, 27);        // Desenha a ultima faixa
    ssd1306_draw_string(&ssd, str_x, 8, 52);           // Desenha o valor em adc
    ssd1306_draw_string(&ssd, str_y, 59, 52);          // Desenha valor em ohm
    ssd1306_send_data(&ssd);                           // Atualiza o display
    sleep_ms(700);
  }
}
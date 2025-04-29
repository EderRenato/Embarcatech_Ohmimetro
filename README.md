# Projeto: Ohmímetro – Raspberry Pi Pico

[![Vídeo de Demonstração](https://img.shields.io/badge/Assista%20no-Google%20Drive-red?logo=google-drive)](https://drive.google.com/file/d/1gwLWkcBT-pigYY4ZQ2yrF0itqHYB4kT3/view?usp=drive_link)

## 📋 Sumário

- [🎯 Objetivo](#🎯-objetivo)
- [⚙️ Funcionalidades Implementadas](#️-funcionalidades-implementadas)
- [🔌 Componentes Utilizados](#-componentes-utilizados)
- [📦 Instruções de Importação e Implementação](#-instruções-de-importação-e-implementação)
- [👨‍💻 Autoria](#-autoria)
- [📝 Descrição do Projeto](#-descrição-do-projeto)

## 🎯 Objetivo

Desenvolver um Ohmímetro utilizando a Raspberry Pi Pico, com foco na medição de resistência de resistores desconhecidos e exibição das informações em um display OLED SSD1306. O projeto visa demonstrar o uso de ADC (Conversor Analógico-Digital) para a leitura de resistores e o cálculo das cores do código de resistores, com base na resistência medida.

## ⚙️ Funcionalidades Implementadas

- Leitura de tensão analógica através do ADC da Raspberry Pi Pico.
- Cálculo de resistência utilizando a fórmula do divisor de tensão.
- Exibição de valores no display OLED SSD1306, incluindo:
  - Valor do ADC.
  - Resistência medida (em ohms).
  - Cores do código de cores de resistores (primeira, segunda e terceira faixa de cores).
- Correção da resistência medida para os valores da série E24.
- Exibição das cores no display com base no valor calculado.

## 🔌 Componentes Utilizados

- Microcontrolador **Raspberry Pi Pico**.
- Display: **OLED SSD1306** (128x64, I2C).
- Resistor conhecido de **10k ohm**.
- **Resistores variados**: O valor da resistência será calculado a partir da leitura do ADC.
- Cabo micro USB para conexão com o computador.

## 📦 Instruções de Importação e Implementação

### 1. Pré-requisitos

- Instale o [Visual Studio Code](https://code.visualstudio.com/).
- Instale a extensão **Raspberry Pi Pico (pico-sdk)** no VS Code.

### 2. Clone o repositório

```bash
git clone https://github.com/SeuUsuario/Ohmimetro-Pico.git
```

### 3. Abra o projeto no VS Code

- Vá em `Arquivo > Abrir Pasta` e selecione a pasta clonada.

### 4. Configure o ambiente

- Siga as instruções da extensão Raspberry Pi Pico para configurar o SDK e o CMake.

### 5. Compile o projeto

- Use o botão **Build** ou pressione `Ctrl+Shift+B`.

### 6. Carregue na placa

- Mantenha o botão **BOOTSEL** pressionado enquanto conecta a Raspberry Pi Pico no PC.
#### Opção 1:
- Após conectar no modo **BOOTSEL**, pressione o botão **Run**.
#### Opção 2:
- Após conectar no modo **BOOTSEL**, copie o arquivo `.uf2` gerado para a unidade que aparecerá.

## 👨‍💻 Autoria

Projeto desenvolvido por **Eder Renato** como parte de um projeto de engenharia utilizando a Raspberry Pi Pico para medir resistências e calcular valores com base no código de cores de resistores.

## 📝 Descrição do Projeto

Este projeto utiliza a Raspberry Pi Pico para medir resistências variadas e exibir os resultados em um display OLED SSD1306. A resistência é calculada com base em uma fórmula do divisor de tensão, e a resistência medida é corrigida para o valor mais próximo da série E24. O código de cores dos resistores também é exibido no display, com base nas faixas de cor.

O objetivo principal deste projeto é demonstrar como utilizar o ADC da Raspberry Pi Pico para interagir com hardware analógico e exibir os resultados de forma visual, promovendo o aprendizado de conceitos de programação embarcada e manipulação de hardware.

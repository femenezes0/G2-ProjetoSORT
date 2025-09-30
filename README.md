# G2 - Projeto SORT: Car Servant

Este projeto consiste no desenvolvimento de um **robô seguidor de linha** com capacidade de realizar curvas de 90° e retomar o percurso em caso de perda da trilha.

## ⚙️ Funcionalidades
- Seguir linha preta em fundo branco.
- Realizar curvas de 90° à esquerda e à direita.
- Procurar a linha quando perdida.

## 🛠️ Tecnologias Utilizadas
- Arduino IDE
- Linguagem C++
- Sensores infravermelhos (5 unidades)
- Motores DC controlados via PWM

## 📂 Estrutura do Código
- Funções de controle de motores (avançar, virar, parar, girar 90°).
- Leitura dos sensores IR (detecção de linha).
- Máquina de estados para decidir entre seguir, virar ou procurar linha.

## 🚀 Como Usar
1. Clone este repositório:
   ```bash
   git clone https://github.com/femenezes0/G2-ProjetoSORT.git


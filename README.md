# Telac

Uma biblioteca/framework para a criação de Interfaces em Modo Texto (TUI), menus complexos e aplicações no terminal rodando integralmente em **C**. 

O **Telac** abstrai toda a complicação de manipular códigos ANSI, renderização iterativa (Immediate Mode GUI), entrada de teclado (inputs sem bloqueio) e redimensionamento dinâmico de terminais. É ideal para quem quer uma experiência próxima ao *ncurses*, mas de forma extremamente simples, direta e controlável através de um loop contínuo.

---

## Como Usar

Basta baixar e colocar os arquivos `telac.c` e `telac.h` junto das fontes do seu projeto. O sistema funciona através de um Main Loop (laço de repetição principal) em "Modo Imediato", e as "rotas" (views) são tradicionalmente controladas pela variável global `tela`.

### Exemplo Básico (Sistema de Gestão)

```c
#include "telac.h"

int main() {
  // Inicializa o motor estrutural, esconde o cursor e abre o loop
  INICIAR_TELA("Sistema Administrativo");

  if (tela == 0) {
      // Cria a renderização de um menu interativo. (Inputs e setinhas são nativos)
      criar_menu(3, "Painel", "Configuracoes", "Sair");
  }

  if (tela == 1) { // Caso o usuário clique em "Painel" na primeira opção
      desenhar_borda(rows, cols);
      
      gotoxy(5, 5);
      print_cor(AZUL, "Resumo Financeiro da Empresa");
      
      // Um botão imediato na posição x=5, y=7
      BOTAO(5, 7, "Voltar ao Inicio") {
          tela = 0; // Volta para o Menu Principal
          limpar();
      }
  }

  // Encerra caso o usuário acesse a tela 3 (Sair)
  if (tela == 3) break; 

  // Desbloqueia o terminal e devolve o histórico
  FINALIZAR_TELA
  return 0;
}
```

### Compilação

Para compilar sua aplicação usando GCC, basta referenciar a biblioteca junto ao seu código base:

```bash
gcc main.c telac.c -o meu_app -Wall
./meu_app
```

---

## Referência Rápida da API

O **Telac** é feito para não ter burocracia. Abaixo estão as ferramentas mais úteis expostas nativamente no `telac.h`:

### Estrutura Base
- `INICIAR_TELA(titulo)`: Macro inteligente que abre o laço de repetição (`while(1)`), ativa a tela limpa alternativa do terminal e inicia a captura *non-blocking* em background das teclas.
- `FINALIZAR_TELA`: Macro obrigatória para fechar o laço de eventos e restaurar o estado real do bash.
- `limpar()`: Pinta todo o terminal de fundo preto.
- `voltar_menu(char tecla)`: Volta automaticamente para a `tela = 0` (Menu) caso o usuário aperte exatamente a tecla declarada.

### Variáveis Globais (O seu Estado)
- `tela`: **(int)** Determina a página/seção exibida no momento. O valor `0` costuma representar o menu inicial.
- `tecla`: **(int)** Guarda a tecla recém-digitada neste exato quadro (Retorna letras como `'a'` ou macros `TECLA_ENTER`, `TECLA_ESC`, `TECLA_DIREITA`, etc).
- `rows` e `cols`: **(int)** Altura e largura atuais do terminal do usuário com atualização ultrarrápida em eventuais redimensionamentos (*resizes*).
- `delay_ms`: **(int)** Mantém a cadência (*framerate*). Pode ser injetada via `definir_fps(fps)`!

### Desenho & Posicionamento
- `gotoxy(int x, int y)`: Leva o "pincel" (cursor invisível) para a coluna (`x`) e linha (`y`) desejada. O topo superior esquerdo é `(1, 1)`.
- `desenhar_borda(int alt, int larg)`: Preenche de modo leve uma moldura pelo canto da tela com hashes (`#`).
- `desenhar_borda_xy(x1, y1, x2, y2)`: Versão mais granular para desenhar dezenas de pequenos quadros pela tela.

### Cores & Tipografia
- `print_cor(Cor cor, formatação...)`: Imprime um texto na cor selecionada (`PRETO, VERMELHO, VERDE, AMARELO, AZUL, MAGENTA, CIANO, BRANCO, PADRAO`). Aceita máscaras flexíveis do C, como texto em variáves `%d`, `%s`.
- `texto_negrito()` | `texto_sublinhado()`: Injeta tags de formatação atrativas *in-line*.
- `resetar_cores()`: Normaliza e desfaz colorações.

### Componentes de Tela (Widgets)
- `criar_menu(qtd, ...)`: Injeta magicamente um supermenu flutuante e já centralizado que roteia o controle das variáveis da `tela`.
- `criar_menu_xy(id, x, y, qtd, ...)`: Cria menus secundários (podem ser vários simultâneos graças ao `id`) customizados que exigem a captura visual com `AO_CLICAR_MENU(id_menu, indice_opcao) { seu código; }`.
- `BOTAO(x, y, "Texto") { seu_codigo }`: Exibe visualmente blocos unitários focáveis que recebem cliques (Interativo usando tabulação lateral de direção).

---
> Desenvolvido primariamente para fins de estudo sobre TUI, Controle Posicional de Cursor e Paradigmas C.

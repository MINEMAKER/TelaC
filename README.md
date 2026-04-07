# Telac

Uma biblioteca/framework leve para a criação de Interfaces em Modo Texto (TUI), menus complexos e aplicações no terminal rodando integralmente em **C**. 

O **Telac** abstrai toda a complicação de manipular códigos ANSI, redimensionamento de terminais e interceptação de teclas, entregando para o desenvolvedor um motor baseado no paradigma *Immediate Mode GUI*.

## Como Usar

Basta carregar os dois arquivos (`telac.c` e `telac.h`) na raiz do seu diretório, adicionar o `#include` e curtir. O sistema inteiro é operado através de uma variável de controle universal chamada `tela`.

### Exemplo Básico (Sistema de Gestão)

```c
#include "telac.h"

int main() {
  // Inicializa o motor estrutural
  INICIAR_TELA("Sistema Administrativo");

  if (tela == 0) {
      // Cria a renderização livre do menu e já captura o input
      criar_menu(3, "Painel de Controle", "Configuracoes da Conta", "Sair");
  }

  if (tela == 1) { // Caso o usuário clique em "Painel de Controle"
      desenhar_borda(rows, cols);
      gotoxy(5, 5);
      print_cor(AZUL, "Resumo Financeiro da Empresa");
      
      // Um botão imediato na posição x=5, y=7
      if (criar_botao(5, 7, "Voltar ao Inicio")) {
          tela = 0; // Volta para o Menu Principal
          limpar();
      }
  }

  // Desbloqueia o terminal e encerra
  FINALIZAR_TELA
  return 0;
}
```

## 🛠️ Compilação

Para compilar sua aplicação usando GCC, basta referenciar a biblioteca junto ao seu código base:

```bash
gcc main.c telac.c -o meu_app
./meu_app
```

---
> Desenvolvido primariamente para fins de estudo sobre TUI, Controle Posicional de Cursor e Paradigmas C.

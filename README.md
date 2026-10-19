# Trabalho Prático 2: O Baralho

**Disciplina:** SCC0223 - Estruturas de Dados 1 (2º Semestre 2026)  
**Instituição:** Instituto de Ciências Matemáticas e de Computação (ICMC - USP)  
**Professora:** Elaine Parros Machado de Sousa  
**Estagiários PAE:** André Magalhães e Matheus Mota  

---

## 📄 Sobre o Projeto

Este projeto implementa um sistema em linguagem C para simulação e manipulação de um baralho virtual com capacidade de $1$ a $100.000$ cartas, processando até $200.000$ comandos por execução. O programa lê comandos através da entrada padrão (`stdin`) e imprime os resultados na saída padrão (`stdout`), encerrando a execução com o comando `FIM`.

O principal objetivo técnico do trabalho é a escolha, implementação e defesa analítica/experimental de estruturas de dados adequadas para resolver operações de movimentação em massa (`CORTA`, `VIRA`, `SOBE`) e consultas rápidas (`CARTA`, `POSICAO`), além de manter um histórico reversível limitado às últimas 100 operações (`DESFAZ`).

---

## 🏗️ Arquitetura e Estruturas de Dados

Para atender aos requisitos do projeto, foram combinadas duas estruturas principais:

### 1. Baralho: Lista Circular Duplamente Encadeada com Nó Sentinela
As cartas do baralho (numeradas de $1$ a $n$) são armazenadas em nós de alocação dinâmica encadeados de forma dupla e circular através de um nó **Sentinela**.

* **Estrutura dos Nós:** Cada nó armazena o rótulo da carta e dois ponteiros (`proximo` e `anterior`).
* **Propriedade Circular e Sentinela:**
  * O topo do baralho é acessível via `sentinela->proximo`.
  * O fundo do baralho é acessível via `sentinela->anterior`.
  * Elimina casos de borda em remoções e inserções no topo ou fundo.
* **Vantagens em Movimentações:**
  * **`CORTA X`:** Realiza o corte rearranjando apenas 4 ponteiros (ponteiros do nó $X$, do seu anterior e da sentinela) com custo $O(1)$ de reconexão de ponteiros após a busca.
  * **`SOBE X`:** Remove o nó da carta $X$ do meio e o reinsere no topo com rearranjo direto de ponteiros $O(1)$.
  * **`VIRA`:** Inverte o sentido de navegação da lista circular/ponteiros.

### 2. Histórico de Operações (`DESFAZ`): Vetor Estático Circular (*Drop-out Stack*)
O histórico armazena as operações que modificam o baralho (`CORTA`, `SOBE`, `VIRA`, `VIRATOPO`) limitado estritamente às últimas **100 operações**.

* **Comportamento LIFO Circular:** Funciona como uma pilha estática de 100 posições com sobrescrita circular. Ao inserir a $101^\text{a}$ operação, a mais antiga é automaticamente sobrescrita sem necessidade de deslocar elementos na memória ($O(1)$).
* **Eficiência de Memória:** Alocada estaticamente, eliminando chamadas a `malloc`/`free` na manipulação do histórico, evitando *overhead* e garantindo ausência de vazamentos de memória (*memory leaks*).
* **Metadados:** Guarda apenas os dados estritamente necessários para desfazer a operação de forma reversa (ex: posição e carta), evitando a duplicação completa do estado do baralho.

---

## 🛠️ Comandos Suportados

| Comando | Descrição | Saída |
| :--- | :--- | :--- |
| `BARALHO n` | Cria um baralho novo com cartas de 1 a $n$. Reseta o histórico. | Nenhuma |
| `CORTA X` | Corta o baralho acima de $X$. $X$ passa a ser o novo topo. | Nenhuma |
| `SOBE X` | Tira a carta $X$ de sua posição e a coloca no topo. | Nenhuma |
| `VIRA` | Inverte totalmente a ordem do baralho. | Nenhuma |
| `VIRATOPO k` | Inverte apenas as $k$ cartas do topo. | Nenhuma |
| `DESFAZ` | Reverte a operação mais recente registrada no histórico. | Nenhuma |
| `CARTA P` | Informa o rótulo da carta presente na posição $P$. | `rótulo` |
| `POSICAO X` | Informa a posição atual em que a carta $X$ se encontra. | `posição` |
| `MOSTRA` | Imprime todas as cartas do topo ao fundo separadas por espaço. | `rótulos...` |
| `FIM` | Finaliza a execução do programa. | Nenhuma |

---

## 💻 Compilação e Execução

O projeto foi desenvolvido em linguagem C utilizando os padrões ANSI C/C11 e a biblioteca padrão (`stdlib.h`, `stdio.h`, `stdbool.h`).

### Compilação via Makefile

Utilize o `Makefile` incluído para compilar o programa principal e o gerador de testes:

```bash
make
```

Isso gera dois executáveis separados:
* `baralho` (ou `programa_principal`): programa principal de simulação.
* `gerador`: programa auxiliar de geração de rajadas de teste para medição.

Para limpar os arquivos objetos e executáveis compilados:

```bash
make clean
```

---

## 📊 Medição de Desempenho e Rajadas

Para gerar arquivos de massa de dados e medir o tempo de execução conforme especificado no enunciado:

1. **Gerar rajada de comandos (exemplo com rajada `SOBE` e $n=50.000$):**
   ```bash
   ./gerador SOBE 50000 > entrada.txt
   ```

2. **Executar medindo o tempo total de processamento:**
   ```bash
   time ./baralho < entrada.txt
   ```

### Perfis de Rajadas Suportados pelo `gerador.c`:
* `SOBE`: comandos `SOBE x` aleatórios.
* `CARTA`: consultas `CARTA p` aleatórias.
* `POSICAO`: consultas `POSICAO x` aleatórias.
* `CORTA`: alternância uniforme entre `CORTA x` e `VIRA`.
* `DESFAZ`: trincas com operação de alteração seguida de `DESFAZ`.

---

## 🔍 Validação de Memória (Valgrind)

A alocação e desalocação dos nós dinâmicos da lista encadeada são gerenciadas para garantir zero vazamentos de memória. Para verificar com o Valgrind:


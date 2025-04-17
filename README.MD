# Simulação de Formação de Moléculas de Água (H₂O)

Este programa simula a formação de moléculas de água (H₂O) utilizando threads e semáforos para coordenar a interação entre átomos de oxigênio (O) e hidrogênio (H). Ele implementa um problema clássico de sincronização em sistemas concorrentes, garantindo que as moléculas sejam formadas corretamente com 1 átomo de oxigênio e 2 átomos de hidrogênio.

## Funcionamento

1. **Átomos de Oxigênio e Hidrogênio**: 
    - Threads representam os átomos de oxigênio e hidrogênio.
    - Cada thread aguarda no "pool" até que os requisitos para formar uma molécula de água sejam atendidos.

2. **Semáforos**:
    - `mutex`: Garante exclusão mútua ao acessar variáveis compartilhadas.
    - `oxygen_queue` e `hydrogen_queue`: Controlam a espera de átomos de oxigênio e hidrogênio.

3. **Formação de Moléculas**:
    - Quando há pelo menos 1 átomo de oxigênio e 2 átomos de hidrogênio no pool, uma molécula de água é formada.
    - O programa exibe animações e atualiza o estado do pool de átomos.

4. **Visualização**:
    - O estado do pool de átomos e o número de moléculas formadas são exibidos no terminal.

## Requisitos

- Sistema operacional Linux ou compatível.
- Compilador GCC.
- Biblioteca `pthread` instalada.

## Como Executar

1. **Clone ou copie o código** para um arquivo chamado `h2o.c`.

2. **Compile o programa** com o seguinte comando:
    ```bash
    gcc -o h2o h2o.c -lpthread
    ```

3. **Execute o programa**:
    ```bash
    ./h2o
    ```

4. **Saída esperada**:
    - O terminal exibirá o estado do pool de átomos, animações de formação de moléculas e o número total de moléculas formadas ao final.

## Exemplo de Saída

```plaintext
=== Simulação de Formação de Água H₂O ===

═══ Pool de Átomos ═══
O: ● ●
  H: ● ● ● ●
Moléculas formadas: 1
✔ Pode formar H₂O!

Formando molécula H₂O #2 ⏳
Formando molécula H₂O #2 ⚡
✔ Molécula H₂O #2 formada!

=== Simulação concluída! ===
Total de moléculas H₂O formadas: 33
```

## Personalização

- **Número de threads**: Altere a constante `NUM_THREADS` para modificar a quantidade de átomos simulados.
- **Velocidade da simulação**: Ajuste os valores de `usleep` para alterar o tempo de espera entre eventos.

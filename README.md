# Simulador Gerenciamento de Memoria
## O objetivo deste exercício é implementar um simulador de gerenciamento de memória de um sistema operacional.

* A memória é controlada por duas listas: Lista de blocos livres e lista de blocos alocados.

* Inicialmente toda a memória está disponível. Quando o usuário solicita a execução de um processo, é reservado
um espaço na memória, que é representado através da alocação de um novo nó na lista de
blocos alocados.

* Antes do processo iniciar a execução, o simulador verifica se há memória disponível. Caso
exista, ele aloca a memória e garante que enquanto o processo estiver executando outro
processo não utilize este espaço na memória. Para cada processo é necessário que exista
um espaço na memória contíguo.

* A memória disponível total é informada no início da main, pela varíavel 'memoria_total'. O simulador
permite as seguintes opções ao usuário: 
  1. Imprime a lista de blocos livres;
  2. Imprime a lista de blocos alocados;
  3. Insere um novo processo;
  4. Remove um processo;
 
* As listas foram implementadas utilizando listas encadeadas.

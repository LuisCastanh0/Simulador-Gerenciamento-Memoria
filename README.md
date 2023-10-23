# Simulador Gerenciamento de Memoria
O objetivo deste exercício é implementar um simulador de gerenciamento de memória de um sistema operacional.

A memória é controlada por duas listas: Lista de blocos livres e lista de blocos alocados.

Inicialmente toda a memória está disponível, quando o usuário solicitar execução de um processo, é reservado
um espaço na memória, isto é representado através da alocação de um novo nó na lista de
memória alocada com exatamente o tamanho de memória necessária para execução do processo.

Antes do processo iniciar a execução, o seu Simulador deve verificar se há memória disponível, caso
exista, deverá alocar de memória e garantir que enquanto o processo estiver executando outro
processo não utilize este espaço na memória, para tanto, devem ser atualizadas as listas de blocos livres
e blocos alocados conforme o exemplo nos próximos slides. Para cada processo é necessário que exista
um espaço na memória contíguo

/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Atividade 5 */
/* Aluno Andre Uziel 119051475 */
/* Codigo: Incrementar de 1 todos os elementos de um vetor */
/* -------------------------------------------------------------------*/

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class Tarefa extends Thread {
  //identificador da thread
  private int id;
  //objeto compartilhado com outras threads
  int[] vetor;
  private int comeco;
  private int fim;
  //construtor
  public Tarefa(int tid, int[] vc, int comeco, int fim) { 
    this.id = tid; 
    this.vetor = vc;
    this.comeco = comeco;
    this.fim = fim;
  }

   //metodo main da thread
  public void run() {
    System.out.println("Thread " + this.id + " iniciou!");
    for (int i=comeco; i<fim; i++) {
      this.vetor[i]++;
    }
    System.out.println("Thread " + this.id + " terminou!"); 
  }
}

//classe da aplicacao
class Atividade5 {
  public static void main (String[] args) {

    if (args.length<2) {
      System.out.println("Digite java <nome_programa> <tamanho_vetor> <nthreads>");
      return;
    }

    int tamanhoVetor = Integer.parseInt(args[0]);
    int nthreads = Integer.parseInt(args[1]);

    if (nthreads <= 0) {
      System.out.println("nthreads invalido");
      return;
    }

    if (nthreads > tamanhoVetor) {
      System.out.println("O numero de threads nao pode ser maior que o tamanho do vetor");
      return;
    }
    
    //cria uma instancia do recurso compartilhado entre as threads
    int[] vetor = new int[tamanhoVetor]; // em java os arrays sao inicializados com 0
    
    System.out.print("Comeco: ");
    for(int i=0;i<tamanhoVetor;i++) { // imprime os valores iniciais
      System.out.printf("%d ",vetor[i]);
    }
    System.out.println("");

    //reserva espaço para um vetor de threads
    Thread[] threads = new Thread[nthreads];

    //cria as threads da aplicacao
    int bloco = tamanhoVetor/nthreads;
    int resto = tamanhoVetor % nthreads;
    int offsetInicial = 0;
    int offsetFinal = 0;
    for (int i=0; i<threads.length; i++) {
      if(resto != 0) {
        offsetFinal++;
      }
      threads[i] = new Tarefa ( i, vetor, (bloco) * i + offsetInicial, (bloco) * (i+1) + offsetFinal );
      if(resto != 0) {
        offsetInicial++;
        resto--;
      }
    }

    //inicia as threads
    for (int i=0; i<threads.length; i++) {
      threads[i].start();
    }

    //espera pelo termino de todas as threads
    for (int i=0; i<threads.length; i++) {
      try { 
        threads[i].join();
      } catch (InterruptedException e) {
        return;
      }
    }
    System.out.print("Fim: ");
    for(int i=0;i<tamanhoVetor;i++) {
      System.out.printf("%d ",vetor[i]);
      if (vetor[i] == 0)
        System.out.println("erro! alguem nao foi incrementado");
    }
  }
}

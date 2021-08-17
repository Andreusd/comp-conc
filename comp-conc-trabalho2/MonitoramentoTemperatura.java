/**
 * Classe principal do programa
 */
class MonitoramentoTemperatura {
  private static int quantidade; //quantidade de atuadores e sensores
  public static boolean debugMode; // flag indicando se estamos debugando

  public static ValorRegistrado[] memoriaCompartilhada; //memoria compartilhada entre as threads
  public static int posicaoAtual; //posicao atual do vetor de memoria compartilhada

  public static void main (String[] args) {
    if(args.length<2) {
      System.out.println("digite java <programa> <quantidade de sensores/atuadores> <modo debug ? 's' / 'n' >");
      return;
    }

    quantidade = Integer.parseInt(args[0]); // recebe a quantidade de sensores/atuadores via linha de comando
    debugMode = args[1].equals("s") ? true : false; // debugMode ?

    LeitorEscritor monitor = new LeitorEscritor();     // Monitor (objeto compartilhado entre leitores e escritores)

    Atuador[] a = new Atuador[quantidade];       // Threads leitores
    Sensor[] s = new Sensor[quantidade];   // Threads escritores
    
    memoriaCompartilhada = new ValorRegistrado[60]; // aloca a memoria compartilhada
    posicaoAtual = 0;

    //inicia o log de saida se em debugMode
    if(debugMode) {
      System.out.println ("import verificaLE_prioridade as VLE");
      System.out.println ("le = VLE.LE()");
    }
    
    for (int i=0; i<quantidade; i++) { // inicializa os atuadores
      a[i] = new Atuador(i+1, 2000, monitor); // delay de 2s
      a[i].start(); 
    }
    for (int i=0; i<quantidade; i++) { // inicializa os sensores
      s[i] = new Sensor(i+1, 1000, monitor); // delay de 1s
      s[i].start(); 
    }
  }
}

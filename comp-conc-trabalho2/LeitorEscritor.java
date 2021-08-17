public class LeitorEscritor {
  private int leit, escr, escritoresQuerendoEscrever; 
  boolean debugMode;

  // Construtor
  LeitorEscritor() { 
    this.leit = 0; //leitores lendo (0 ou mais)
    this.escr = 0; //escritor escrevendo (0 ou 1)
    escritoresQuerendoEscrever = 0;
    this.debugMode = MonitoramentoTemperatura.debugMode;
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      while (this.escr > 0 || this.escritoresQuerendoEscrever > 0) {
        if(debugMode) System.out.println ("le.leitorBloqueado("+id+")");
        wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      if(debugMode) System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
    this.leit--; //registra que um leitor saiu
    if (this.leit == 0) 
      this.notify(); //libera escritor (caso exista escritor bloqueado)
    if(debugMode) System.out.println ("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
        this.escritoresQuerendoEscrever++;
        if(debugMode) System.out.println ("le.escritorBloqueado("+id+")");
        wait();  //bloqueia pela condicao logica da aplicacao 
        this.escritoresQuerendoEscrever--; // escritor desbloqueado
        if(debugMode) System.out.println ("le.escritorDesbloqueado("+id+")"); // necessario para verificar a corretude em python
      }
      this.escr++; //registra que ha um escritor escrevendo
      if(debugMode) System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
    this.escr--; //registra que o escritor saiu
    notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    if(debugMode) System.out.println ("le.escritorSaindo("+id+")");
  }
}


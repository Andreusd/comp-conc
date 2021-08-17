import java.util.Random;

public class Sensor extends Thread {
  private int id; //identificador da thread
  private int delay; //atraso bobo...
  private int leitura; //contador de leitura
  private LeitorEscritor monitor; //objeto monitor para coordenar a lógica de execução das threads
  private Random random; //randomizador de temperatura
  private boolean debugMode;

  // Construtor
  Sensor (int id, int delay, LeitorEscritor m) {
    this.id = id;
    this.delay = delay;
    this.monitor = m;
    random = new Random(); // objeto auxiliar para gerar valores aleatórios
    this.leitura=0; // identificador de leitura
    this.debugMode = MonitoramentoTemperatura.debugMode;
  }

  // Método executado pela thread
  public void run () {
    try {
      while(true) {
        this.monitor.EntraEscritor(this.id);
        int aleatorio = random.nextInt(16)+25; // gera um numero entre 25 e 40
        if(aleatorio > 30) { // maior que 30?
          ValorRegistrado valor = new ValorRegistrado(aleatorio, id, leitura); // objeto auxiliar
          MonitoramentoTemperatura.memoriaCompartilhada[MonitoramentoTemperatura.posicaoAtual % 60] = valor; // guarda esse valor
          if(debugMode) System.out.printf("# Sensor %d escreveu %d em %d\n",id,aleatorio,MonitoramentoTemperatura.posicaoAtual % 60);
          MonitoramentoTemperatura.posicaoAtual++; // aumenta a posicao atual
        }
        // FuncoesDebug.imprimeVetorDeTemperatura(); //debug
        leitura++; // aumenta o identificador de leitura
        this.monitor.SaiEscritor(this.id); 
        sleep(this.delay); //intervalo de medicao de temperatura
      }
    } catch (InterruptedException e) { return; }
  }
}

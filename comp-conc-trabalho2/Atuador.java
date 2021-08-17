public class Atuador extends Thread {
  int id; // identificador da thread
  int delay; // intervalo de leitura
  LeitorEscritor monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Atuador (int id, int delay, LeitorEscritor m) {
    this.id = id;
    this.delay = delay;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
      while(true) {
        this.monitor.EntraLeitor(this.id);
        int contExtremo = 0; // variavel usada para contar as temperaturas de mesmo id acima de 35
        int contadorTotal=0; // contador de temperaturas de mesmo id
        double somaTemp=0; // somador de temperaturas com mesmo id
        boolean alertaVermelho=false; // variavel auxiliar para indicar alerta vermelho
        boolean alertaAmarelo=false; // variavel auxiliar para indicar alerta amarelo
        int posicaoAtual = MonitoramentoTemperatura.posicaoAtual; // variavel auxiliar para economizar espaço
        for(int i=(posicaoAtual-1); i >= 0 && i > (posicaoAtual-61) ; i--) { //o loop da um ciclo na memoria, mas se for a primeira vez, so ate o inicio
          if(MonitoramentoTemperatura.memoriaCompartilhada[i % 60].identificadorDoSensor == this.id) { // mesmo id ?
            if(MonitoramentoTemperatura.memoriaCompartilhada[i % 60].temperaturaRegistrada > 35) // temperatura extrema?
              contExtremo++;
            if(contadorTotal==5 && contExtremo==5) { // alerta vermelho! as ultimas 5 temperaturas sao extremas!
              alertaVermelho=true;
            }
            if(contadorTotal==15 && contExtremo>=5) { // alerta amarelo, entre as 15 ultimas, 5 sao extremas!
              alertaAmarelo=true;
            }
            contadorTotal++;
            somaTemp += MonitoramentoTemperatura.memoriaCompartilhada[i % 60].temperaturaRegistrada;
          }
        }
        double media;
        if(contadorTotal==0) // 0 dividido por 0 vou considerar 0
          media=0;
        else
          media=somaTemp/contadorTotal; // calcula a media

        if(alertaVermelho)
          System.out.printf("# >>>ALERTA VERMELHO<<< --- MEDIA: %.2f ID: %d\n",media,this.id);
        else if(alertaAmarelo)
          System.out.printf("# ALERTA AMARELO --- MEDIA: %.2f ID: %d\n",media,this.id);
        else
          System.out.printf("# CONDICAO NORMAL --- MEDIA: %.2f ID: %d\n",media,this.id);
        this.monitor.SaiLeitor(this.id);
        sleep(this.delay); 
      }
    } catch (InterruptedException e) { return; }
  }
}


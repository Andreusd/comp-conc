public class FuncoesDebug {
  /**
   * funcao auxiliar que imprime a temperatura e o identificador associado
   */
  public static void imprimeVetorDeTemperatura() { 
    for(var item : MonitoramentoTemperatura.memoriaCompartilhada) {
      if(item == null) //evita a impressão de índices ainda não preenchidos
        break;
      System.out.printf("%d:%d ",item.temperaturaRegistrada,item.identificadorDoSensor);
    }
    System.out.println("");
  }
}

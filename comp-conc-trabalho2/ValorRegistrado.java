/**
 * classe auxiliar para armazenar 3 dados de uma so vez
 */
public class ValorRegistrado {
  public int temperaturaRegistrada;
  public int identificadorDoSensor;
  public int identificadorDeLeitura;

  public ValorRegistrado(int temp,int idSensor,int idLeitura) {
    this.temperaturaRegistrada = temp;
    this.identificadorDoSensor = idSensor;
    this.identificadorDeLeitura = idLeitura;
  }
}

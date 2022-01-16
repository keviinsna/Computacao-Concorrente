/* Código: Estendendo a classe Thread de Java
 */

// Passo 1: Cria uma classe que estende a classe Thread
class Ola extends Thread{
	private String msg;

	// Construtor
	Ola(String msg){
		this.msg = msg;
	}

	// Método executado pela thread
	public void run(){
		System.out.println(msg);
	}
}

// Classe do método java
class OlaThread{
	static final int N = 10;

	public static void main(String[] args){
		// Reserva espaço para um vetor de threads
		Thread[] threads = new Thread[N];

		// Passo 2: Cria as threads da classe que estende Thread
		for(int i = 0; i < N; i++){
			final String m = "Olá da thread " + i;
			threads[i] = new Ola(m);
		}

		// Passo 3: Inicia as threads
		for(int i = 0; i < N; i++){
			threads[i].start();
		}

		// Passo 4: Espera pelo término das threads
		for(int i = 0; i < N; i++){
			try {threads[i].join();}
			catch (InterruptedException e){return ;}
		}

		System.out.println("Terminou");
	}
}

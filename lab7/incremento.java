/* Código: Acessando variável compartilhada em um programa multithreading Java */

// Classe da estrutura de dados (recurso) compartilhado entre as threads
class S{
	private int r; // Recurso compartilhado

	// Construtor
	S(){
		this.r = 0;
	}
	
	// Não trata a condição de corrida

	// Operação de escrita sobre o recurso compartilhado
	/*public void inc(){
		this.r++;
	}

	// Operação de leitura sobre o recurso compartilhado
	public int get(){
		return this.r;
	}*/

	// ou, tratando a condição de corrida
	
	
	public synchronized void inc(){
		this.r++;
	}

	public synchronized int get(){
		return this.r;
	}
	
}

// Classe que estende Thread e implementa a tarefa de cada thread do programa
class T extends Thread{
	private int id; // Identificador da thread
	S s; // Objeto compartilhado com outras threads

	// Construtor
	public T(int tid, S s){
		this.id = tid;
		this.s = s;
	}

	// Método main da thread
	public void run(){
		System.out.println("Thread " + this.id + " iniciou!");
		for(int i = 0; i < 100_000; i++){
			this.s.inc();
		}
		System.out.println("Thread " + this.id + " terminou!");
	}

}

// Classe da aplicação
class IncrementoBase{
	static final int N = 2;

	public static void main(String[] args){
		// Reserva espaço para um vetorde threads
		Thread[] threads = new Thread[N];

		// Cria uma instância do recruso compartilhado entre as threads
		S s = new S();

		// Cria as threads
		for(int i = 0; i < N; i++){
			threads[i] = new T(i, s);
		}

		// Inicia as threads
		for(int i = 0; i < N; i++){
			threads[i].start();
		}

		// Espera pelo término das threads
		for(int i = 0; i < N; i++){
			try{threads[i].join();}
			catch(InterruptedException e){return;}
		}

		System.out.println("Valor de s = " + s.get());
	}
}




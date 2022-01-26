/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 2  - Laboratório 8
 * Atividade:  Implementar uma aplicação concorrente em Java usando
 * 	       o padrão leitores e escritores.
 * 	       Leitores: lê a variável e imprime se é par ou ímpar.
 * 	       Escritores: escreve na variável o id da thread.
 * 	       LeitoresEscritores: lê, imprime e incrementa a variável.
 */

// Classe monitor que implementa a lógica leitores/escritores
class EstLE{
	private int leit, escr;
	private int v;		// Variável inteiro compartilhada
	
	// Construtor
	public EstLE(){
		this.leit = 0;
		this.escr = 0;
	
	}

	// Entrada para leitores
	public synchronized void entradaLeitores(int id, boolean le){
		try{
			while(this.escr > 0){
				/*
				if(le)
					System.out.println("le.leitorEscritorBloqueadoLeitura(" + id + ")");
				else
					System.out.println("le.leitorBloqueado(" + id + ")");
				*/
				wait();
			}
			this.leit++; // Registra que há mais um leitor lendo
			
			/*
			if(le)
				System.out.println("le.leitorEscritorLendo(" + id + ")");
			else
				System.out.println("le.leitorLendo(" + id + ")");
			*/

		}catch(InterruptedException e){return;}
	}

	// Saída para leitores
	public synchronized void saidaLeitores(int id, boolean le){
		this.leit--; // Registra que um leitor saiu
		if(this.leit == 0)
			notify(); // Libera escritores e leitoresEscritores
		/*
		if(le)
			System.out.println("le.leitorEscritorSaindoLeitura(" + id + ")");
		else
			System.out.println("le.leitorSaindo(" + id + ")");
		*/
	}

	// Entrada para escritores
	public synchronized void entradaEscritores(int id, boolean le){
		try{
			while(this.escr > 0 || this.leit > 0){
			//if(this.escr > 0 || this.leit > 0){
				/*
				if(le)
					System.out.println("le.leitorEscritorBloqueadoEscrita(" + id + ")");
				else
					System.out.println("le.escritorBloqueado(" + id + ")");
				*/

				wait();
			}
			this.escr++; // Registra que há mais um escritor escrevendo
			
			/*
			if(le)
				System.out.println("le.leitorEscritorEscrevendo(" + id + ")");
			else
				System.out.println("le.escritorEscrevendo(" + id + ")");
			*/

		}catch(InterruptedException e){return;}
	}

	// Saída para escritores
	public synchronized void saidaEscritores(int id, boolean le){
		this.escr--;  // Registra que o escritor saiu
		notifyAll(); //  Libera todos os leitores e escritores
	
		/*
		if(le)
			System.out.println("le.leitorEscritorSaindoEscrita(" + id + ")");
		else
			System.out.println("le.escritorSaindo(" + id + ")");
		*/
	}

	// Operações com a variável compartilhada
	public synchronized void inc(){
		this.v++;
	}

	public synchronized void set(int id){
		this.v = id;
	}

	public synchronized int get(){
		return this.v;
	}

}

// Classe de leitores
class Leitor extends Thread{
	private int id;      // Identificador da thread
	private EstLE monitor; //  Objeto monitor para coordenar a lógica da aplicação

	// Costrutor
	public Leitor(int id, EstLE monitor){
		this.id = id;
		this.monitor = monitor;
	}

	// Método executado pelas threads
	public void run(){
		for(;;){
			this.monitor.entradaLeitores(this.id, false);
				
			// Lê e imprime se é par ou ímpar
			
			int k = this.monitor.get();
			if(k % 2 == 0)
				System.out.println("L[" + id + "]: " + k + " é par!");
			else
				System.out.println("L[" + id + "]: " + k + " é ímpar!");
			
			this.monitor.saidaLeitores(this.id, false);
			
			// Processamento bobo para não pegar CPU muito rápido
			k = 1_000_000_000;
			while(k > 0) k--;

		}
	}
}

// Classe de escritores
class Escritor extends Thread{
	private int id;	     // Identificador da thread
	private EstLE monitor; //  Objeto monitor para coordenar a lógica da aplicação

	// Costrutor
	public Escritor(int id, EstLE monitor){
		this.id = id;
		this.monitor = monitor;
	}

	// Método executado pelas threads
	public void run(){
		for(;;){
			this.monitor.entradaEscritores(this.id, false);

			System.out.println("E[" + id + "] troca V pelo identificador da thread.");

			// Modifica a variável escrevendo o valor do id da thread
			this.monitor.set(this.id);
	
			this.monitor.saidaEscritores(this.id, false);
			
			// Processamento bobo para não pegar CPU muito rápido
			int k = 1_000_000_000;
			while(k > 0) k--;

		}	
	}
}

// Classe de leitores e escritores
class LeitorEscritor extends Thread{
	private int id; 	 // Identificador da thread
	private EstLE monitor;	//  Objeto monitor para coordenar a lógica da aplicação

	// Costrutor
	public LeitorEscritor(int id, EstLE monitor){
		this.id = id;
		this.monitor = monitor;
	}

	// Método executado pelas threads
	public void run(){
		for(;;){
			
			// Lê e imprime  a variável
			this.monitor.entradaLeitores(this.id, true);
			
			int k = this.monitor.get();
			System.out.println("LE[" + id + "]: V = " + k);
			
			this.monitor.saidaLeitores(this.id, true);
			
			// Processamento bobo
			k = 1_000_000_000;
			while(k > 0) k--;

			// Incrementa a variável
			this.monitor.entradaEscritores(this.id, true);
			
			System.out.println("LE[" + id + "] incrementa o valor");
			this.monitor.inc();

			this.monitor.saidaEscritores(this.id, true);
		}
	}
}


// Class principal
class Principal{
	static final int  L = 2;
	static final int  E = 3;
	static final int LE = 2;

	public static void main(String[] args){
		EstLE monitor = new EstLE(); // Monitor (objeto compartilhado)
		
		Escritor[] escritores = new Escritor[E]; 		         // Threads escritoras
		Leitor[] leitores = new Leitor[L];				//  Threads leitoras
		LeitorEscritor[] leitoresEscritores = new LeitorEscritor[LE];  //   Threads leitoras e escritoras

		// Inicia o log das saídas
		System.out.println("import verificaLE");
		System.out.println("le = verificaLE.LE()");

		// Cria e inicia as threads
		for(int i = 0; i < L; i++){
			leitores[i] = new Leitor(i + 1, monitor);
			leitores[i].start();
		}

		for(int i = 0; i < E; i++){
			escritores[i] = new Escritor(i + 1, monitor);
			escritores[i].start();
		}
		
		for(int i = 0; i < LE; i++){
			leitoresEscritores[i] = new LeitorEscritor(i + 1, monitor);
			leitoresEscritores[i].start();
		}	
	}
}

/* Código: Leitores e escritores usando monitores em Java
 */


// Monitor que implementa a lógica do padrão leitores/escritores
class LE{
	private int leit, escr;

	// Construtor
	LE(){
		this.leit = 0;
		this.escr = 0;
	}

	// Entrada para leitores
	public synchronized void entradaLeitor(int id){
		try{
			while(this.escr > 0){ // Se tiver escritor escrevendo
			//if(){
				System.out.println("le.leitorBloqueado(" + id + ")");
				wait(); // Bloqueia pela condição lógica da aplicação
			}
			this.leit++; // Registra que há mais um leitor lendo
			System.out.println("le.leitorLendo(" + id + ")");
		}catch(InterruptedException e){return;}
	}

	// Saída para leitores
	public synchronized void saidaLeitor(int id){
		if(--this.leit == 0)
			this.notify(); // Libera algum escritor, caso tenha
		System.out.println("le.leitorSaindo(" + id + ")");
	}

	// Entrada para escritores
	public synchronized void entradaEscritor(int id){
		try{
			while(this.leit > 0 || this.escr > 0){
			//if(this.leit > 0 || this.escr > 0){
				System.out.println("le.escritorBloqueado(" + id + ")");
				wait(); // Bloqueia pela condição lógica da aplicação
			}
			this.escr++;
			System.out.println("le.escritorEscrevendo(" + id + ")");
		}catch(InterruptedException e){return;}
	}

	// Saída para escritores
	public synchronized void saidaEscritor(int id){
		this.escr--; // Registra que o escritor saiu
		notifyAll(); // Libera todos os leitores e escritores
		System.out.println("le.escritorSaindo(" + id + ")");
	}
}


// Aplicação de exemplo
class Leitor extends Thread{ // Leitor
	private int id; // Identificador da thread
	private int delay; // Delay bobo
	private LE monitor; // Objeto monitor para coordenar a lógica da aplicação

	// Construtor
	Leitor(int id, int delayTime, LE m){
		this.id = id;
		this.delay = delayTime;
		this.monitor = m;
	}

	// Método executado pelas threads
	public void run(){
		double j = 7777777777.7, i;
		try{
			for(;;){
				this.monitor.entradaLeitor(this.id);
				
				for(i = 0; i < 1_000_000; i++) j /= 2;
				
				this.monitor.saidaLeitor(this.id);
				sleep(this.delay);
			}
		}catch(InterruptedException e) {return;}
	}

}

class Escritor extends Thread{ // Escritor
	private int id; // Identificador da thread
	private int delay; // Delay bobo
	private LE monitor; // Objeto monitor para coordenar a lógica da aplicação

	// Construtor
	Escritor(int id, int delayTime, LE m){
		this.id = id;
		this.delay = delayTime;
		this.monitor = m;
	}

	// Método executado pelas threads
	public void run(){
		double j = 7777777777.7, i;
		try{
			for(;;){
				this.monitor.entradaEscritor(this.id);
				
				for(i = 0; i < 1_000_000; i++) j /= 2;
				
				this.monitor.saidaEscritor(this.id);
				sleep(this.delay);
			}
		}catch(InterruptedException e) {return;}
	}

}

// Classe principal
class LeitorEscritor{
	static final int L = 4;
	static final int E = 3;

	public static void main(String[] args){
		LE monitor = new LE(); // Monitor (objeto compartilhado)
		Escritor[] escritores = new Escritor[E]; // Threads escritoras
		Leitor[] leitores = new Leitor[L]; // Threads leitoreas
		
		// Inicia o log das saídas
		System.out.println("import verificaLE");
		System.out.println("le = verificaLE.LE()");

		// Cria e inicia as threads
		for(int i = 0; i < L; i++){
			leitores[i] = new Leitor(i + 1, (i + 1) * 500, monitor);
			leitores[i].start();
		}
		for(int i = 0; i < E; i++){
			escritores[i] = new Escritor(i + 1, (i + 1) * 500, monitor);
			escritores[i].start();
		}

	}
}

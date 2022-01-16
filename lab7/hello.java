/* Código: "Hello, world" usando threads em java, implmentando
 * a interface Runnable.
 */


// Passo 1: Criar uma classe que implementa a interface Runnable
class Hello implements Runnable{
	private String msg;

	// Construtor
	Hello(String msg){
		this.msg = msg;
	}
	
	// Método executado pelas threads provido pela interface
	public void run(){
		System.out.println(msg);
	}
}

// Classe main
class HelloThread{	
	public static final int N = 10;
	public static void main(String[] args){
		// Reserva espaço para um vetor de threads
		Thread[] threads = new Thread[N];
		
		// Passo 2: Transforma o objeto Runnable em Thread
		// Apenas prepara as threads.
		for(int i = 0; i < N; i++){
			final String m = "Hello world da thread" + i;
			System.out.println("Cria a thread " + i);

			// O objeto Thread recebe como parâmetro um objeto Runnable
			threads[i] = new Thread(new Hello(m));
		}

		// Passo 3: Iniciar as threads
		for(int i = 0; i < N; i++){
			threads[i].start();
		}

		// Passo 4: Espera pelo término das threads
		 for(int i = 0; i < N; i++){
		 	try{
				threads[i].join();
			}catch(InterruptedException e) {return ;}
		 }
		

		// Em java não precisa do "pthread_exit()" como em C.

		System.out.println("Terminou");
	}
}

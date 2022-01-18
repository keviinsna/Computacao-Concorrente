/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 2  - Laboratório 7
 * Atividade:  Implementar um programa concorrente em Java
 * 	       para somar todos os elementos de um vetor de
 * 	       inteiros. Divida a tarefa entre as threads de
 * 	       forma balanceada e garanta ausência de condição
 * 	       de corrida. 
 */

import java.util.Random;

// Classe da estrutura compartilhada entre as threads
class Est{
	private int soma; // Variável que acumula as somas
	private int[] array; // Array compartilhado

	// Construtor
	public Est(int[] array){
		this.soma = 0;
		this.array = array;
	}

	// Método que soma elemento à variável soma
	public synchronized void add(int n){
		this.soma += n;
	}

	// Método que retorna variável soma
	public synchronized int get(){
		return this.soma;
	}

	public int[] getArray(){
		return this.array;
	}

}

// Classe que estende de Thread e implementa a tarefa de cada thread
class T extends Thread{
	private int id; // Identificador da thread
	Est est; // Objeto compartilhado com outras threads

	private int[] arrayLocal;
	private int nthreads;

	// Construtor
	public T(int id, Est est, int nthreads){
		this.id = id;
		this.est = est;
		this.nthreads = nthreads;
		this.arrayLocal = null;	
	}

	// Método main da thread
	public void run(){
		arrayLocal = this.est.getArray();
		System.out.println("Thread " + this.id + " vai somar!");
		for(int i = this.id; i < this.arrayLocal.length; i+=this.nthreads){
			this.est.add(arrayLocal[i]);
		}
		System.out.println("Thread " + this.id + " somou!");
	}
}

// Classe principal da aplicação
class Somatorio{
	static final int NTHREADS = 4;
	static final int TAM = 100_000_000;

	public static void main(String[] args){
		int somaSequencial = 0;
		int[] vet = new int[TAM];
		Thread[] threads = new Thread[NTHREADS];

		// Inicializa o array
		Random rand = new Random();
		for(int i = 0; i < TAM; i++){
			vet[i] = rand.nextInt(11);
			somaSequencial += vet[i];
		}

		// Cria uma instância da estrutura compartilhada
		Est est = new Est(vet);

		// Cria as threads
		for(int i = 0; i < NTHREADS; i++){
			threads[i] = new T(i, est, NTHREADS);
		}

		// Inicia as threads
		for(int i = 0; i < NTHREADS; i++){
			threads[i].start();
		}

		// Espera pelo término das threads
		for(int i = 0; i < NTHREADS; i++){
			try{threads[i].join();}
			catch(InterruptedException e){return;}
		}

		if(somaSequencial == est.get())
			System.out.println("As threads somaram corretamente: " + est.get());
		else
			System.out.println("As somas não bateram");

	}
}

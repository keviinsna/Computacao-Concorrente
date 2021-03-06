#Define funções para verificar a lógica de execução de uma aplicação leitor/escritor


class LE:
    def __init__(self):
        self.escritores = 0;
        self.leitores = 0;

    def leitorBloqueado(self, id):
        '''Recebe o id do leitor. Verifica se a decisão de bloqueio está correta.'''
        if(self.escritores == 0):
            print("ERRO: leitor " + str(id) + " bloqueado quando não há escritores")

    def escritorBloqueado(self, id):
        '''Recebe o id do escritor. Verifica se a decisão de bloqueio está correta.'''
        if(self.escritores == 0) and (self.leitores == 0):
            print("ERRO: escritor " + str(id) + " bloqueado quando não há escritores e nem leitores")

    def leitorEscritorBloqueadoLeitura(self, id):
        '''Recebe o id do leitor/escritor e verifica se a decisão de bloqueio está correta.'''
        if(self.escritores == 0):
            print("ERRO: leitor " + str(id) + " bloqueado quando não há escritores")
    
    def leitorEscritorBloqueadoEscrita(self, id):
        '''Recebe o id do leitor/escritor e verifica se a decisão de bloqueio está correta.'''
        if(self.escritores == 0) and (self.leitores == 0):
            print("ERRO: leitor " + str(id) + " bloqueado quando não há escritores e nem leitores")

    def leitorLendo(self, id):
        '''Recebe o id do leitor. Verifica se pode ler a registra que está lendo.'''
        if(self.escritores > 0):
            print("ERRO: leitor " + str(id) + " está lendo quando há escritor escrevendo")
        self.leitores += 1

    def escritorEscrevendo(self, id):
        '''Recebe o id do escritor. Verifica se pode escrever e registra que está escrevendo.'''
        if(self.escritores > 0) or (self.leitores > 0):
            print("ERRO: escritor " + str(id) + " está escrevendo quando há escritores escrevendo ou leitores lendo")
        self.escritores += 1

    def leitorEscritorLendo(self, id):
        '''Recebe o id do leitor/escritor e verifica se pode ler e registra que está lendo.'''
        if(self.escritores > 0):
            print("ERRO: leitor/escritor " + str(id) + " lendo quando há escritores escrevendo")
        self.leitores += 1

    def leitorEscritorEscrevendo(self, id):
        '''Recebe o id do leitor/escritor e verifica se pode escrever e registra que está escrevendo.'''
        if(self.leitores > 0) or (self.escritores > 0):
            print("ERRO: leitor/escritor " + str(id) + " escrevendo quando há escritores escrevendo ou leitores lendo")
        self.escritores += 1

    def leitorSaindo(self, id):
        '''Recebe o id do leitor e registra se terminou a leitura.'''
        self.leitores -= 1

    def escritorSaindo(self, id):
        '''Recebe o id do escritor e registra se terminou a escrita'''
        self.escritores -= 1
    
    def leitorEscritorSaindoLeitura(self, id):
        '''Recebe o id do leitor/escritor e registra se terminou a leitura e a escrita.'''
        self.leitores -= 1
    
    def leitorEscritorSaindoEscrita(self, id):
        '''Recebe o id do leitor/escritor e registra se terminou a escrita'''
        self.escritores -= 1
    

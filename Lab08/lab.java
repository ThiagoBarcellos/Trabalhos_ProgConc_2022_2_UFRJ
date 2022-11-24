class BufferLimitado {
    static final int N = 10; // tamanho do buffer
    private int[] buffer = new int[N];
    private int count = 0, in = 0, out = 0;

    //construtor
    BufferLimitado(){
        for(int i = 0; i<N; i++){
            buffer[i] = 0;
        }
    }

    public void Imprime () {
        for(int i = 0; i < N; i++){
                System.out.print(buffer[i]);
        }
        System.out.println("");
    }

    public synchronized void Insere (int elemento){
        try{
            while(count == N){
                wait();
            }
            buffer[in] = elemento;
            in = (in+1)%N;
            count ++;
            Imprime();
            notifyAll();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public synchronized int Remove (){
        int elemento;
        try{
            while(count == 0){
                wait();
            }
            elemento = buffer[out];
            buffer[out] = 0;
            out = (out+1)%N;
            count--;
            Imprime();
            notifyAll();
            return elemento;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return 0;
    }
}

class Produtor extends Thread{
    int id; //identificador da thread
    int delay; //atraso
    BufferLimitado monitor;

    //construtor
    Produtor(int id, int delayTime, BufferLimitado m){
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    //método executado pela thread
    public void run () {
        int elemento = 3;
        while(true){
        try{
            monitor.Insere(elemento);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        }
    }
}

class Consumidor extends Thread {
    int id;
    int delay;
    BufferLimitado monitor;

    //construtor
    Consumidor(int id, int delayTime, BufferLimitado m){
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    //método executado pela thread
    public void run () {
        int elemento;
        while (true){
            try{
                elemento = monitor.Remove();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}

class lab{
    static final int P = 1;
    static final int C = 1;

    public static void main (String[] args){
        int i;
        BufferLimitado monitor = new BufferLimitado();
        Produtor[] p = new Produtor[P]; //Threads produtoras
        Consumidor[] c = new Consumidor[C]; //Threads consumidoras
        for(i = 0; i<P; i++){
            p[i] = new Produtor(i+1, (i+1)*500, monitor);
            p[i].start();
        }
        for(i = 0; i<C; i++){
            c[i] = new Consumidor(i+1, (i+1)*500, monitor);
            c[i].start();
        }
    }
}


#import <queue>
#import <exception>

template<typename T>
class Processor<T>{
private:
    std::thread internal_thread;
    std::queue<T> jobs;
    std::function<void(T)> f;
    T value;
    bool closed;
    std::mutex m;   //when all data are finished, notify close()
public:
    Processor(std::function<void(T)> f): f(f), closed(false) {
        auto f=[&](){
            while(true){
                std::unique_lock<std::mutex> ul(m);
                cv.wait(ul, [this](){ return (!jobs.empty() || closed)};
                //quando processor non è chiuso e ci sono elementi in coda ne estraggo uno e faccio partire il thread interno con quello
                if(closed)
                    break;
                
                value=jobs.front();
                jobs.pop_front();
                f(value);
            }
            
        }
        std::thread temp_thread(f);
        
        
        //esco dal 
        internal_thread=std::move(f(t));
    }
    
    ~Processor(){
        t.join();
    }
    
    void send(T t){
        std::unique_lock<std::mutex> ul(m);        
        if(closed)
            throw std::runtime_exception("Processor was closed!");
        
        //processor still open;
        jobs.push(t);
        cv.notify_one();
 
    }
    
    void close(){
        //questa invocazione implica che future chiamate a send lanceranno un'eccezione
        //ritorna solo quando tutti i dati sono stati elaborati
        std::unique_lock<std::mutex> ul(m);        
        closed=true;
        cv.notify_one();
        if(t.joinable())
            t.join();
        return;  
    }
}
// T copiabile
// Processor contiene un thread consumatore che viene avviato alla creazione di Processor che elabora gli oggetti ricevuti usando la funzione passata al costruttore f.

// produttori possono chiamare Processor p.send(obj) per mandare loro oggetti 
da elaborare.
// gli oggetti ricevuti possono essere temporaneamente messi in una coda in attesa di elaborazione.

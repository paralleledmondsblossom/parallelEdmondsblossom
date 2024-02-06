#ifndef Matcher_H
#define Matcher_H

#include "Graph.h"
#include "Vertex.h"
#include <list>
#include <unordered_map>
#include "Enums.h"
#include "DSU.h"
//#include "DSU2.h"

#include "Blossom.h"
#include "Stack.h"
#include "Frontier.h"
#include "Statistics.h"
#include "concurrentqueue.h"

class Matcher {
public:
    template <typename IT, typename VT>
    static void match(Graph<IT, VT>& graph);
    template <typename IT, typename VT>
    static void match(Graph<IT, VT>& graph, Statistics<IT>& stats);
    template <typename IT, typename VT>
    static void match_wl(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold);
    template <typename IT, typename VT>
    static void match_wl2(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold);
    template <typename IT, typename VT>
    static void match_wl3(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold);
    template <typename IT, typename VT>
    static void match_persistent_wl(Graph<IT, VT> &graph,
                                    moodycamel::ConcurrentQueue<IT> &worklist,
                                    bool &finished);
    template <typename IT, typename VT>
    static void match_persistent_wl2(Graph<IT, VT> &graph,
                                    std::vector<moodycamel::ConcurrentQueue<IT, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                    moodycamel::ConcurrentQueue<IT> &worklist,
                                    std::vector<size_t> &read_messages,
                                    std::atomic<bool>& found_augmenting_path,
                                    std::atomic<IT> & currentRoot,
                                    std::vector<std::mutex> &worklistMutexes,
                                    std::vector<std::condition_variable> &worklistCVs,
                                    int tid,
                                    std::atomic<IT> & num_enqueued,
                                    std::atomic<IT> & num_dequeued,
                                    std::atomic<IT> & num_contracting_blossoms);

template <typename IT, typename VT>
static void match_persistent_wl4(Graph<IT, VT>& graph,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<bool>& found_augmenting_path,
                                std::atomic<IT> & currentRoot,
                                std::vector<std::mutex> &worklistMutexes,
                                std::vector<std::condition_variable> &worklistCVs,
                                int tid);

template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
static void match_persistent_wl5(Graph<IT, VT>& graph,
                                std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<bool>& found_augmenting_path,
                                std::atomic<IT> & currentRoot,
                                std::vector<std::mutex> &worklistMutexes,
                                std::vector<std::condition_variable> &worklistCVs,
                                int tid,
                                std::atomic<IT> & num_enqueued,
                                std::atomic<IT> & num_dequeued,
                                std::atomic<IT> & num_contracting_blossoms,
                                int deferral_threshold);
template <typename IT, typename VT>
static void match_persistent_wl6(Graph<IT, VT>& graph,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                std::vector<std::atomic_flag> &dead,
                                std::atomic<bool> &finished,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<IT> & currentRoot,
                                int tid,
                                int deferral_threshold);

template <typename IT, typename VT>
static void match_persistent_wl7(Graph<IT, VT>& graph,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                std::vector<std::atomic_flag> &dead,
                                std::atomic<bool> &finished,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<IT> & currentRoot,
                                int tid,
                                int deferral_threshold);
private:
    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
    static void search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector);

    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
    static bool search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    IT deferral_threshold);

    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
    static void continue_search(Graph<IT, VT>& graph, 
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector);

    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType = std::vector>
    static bool concurrent_search(Graph<IT, VT>& graph, 
                        FrontierType<IT, StackType> & f,
                        std::vector<Vertex<IT>> & vertexVector);

    template <typename IT, typename VT>
    static bool capped_search(Graph<IT, VT>& graph, 
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    int max_depth);

    template <typename IT, typename VT>
    static void start_search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::atomic<IT> & num_enqueued,
                    std::vector<moodycamel::ConcurrentQueue<Frontier<IT>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                    std::atomic<bool>& found_augmenting_path,
                    std::atomic<IT> &masterTID);
    template <typename IT, typename VT>
    static void continue_search(Graph<IT, VT>& graph, 
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::atomic<IT> & num_enqueued,
                    std::vector<moodycamel::ConcurrentQueue<Frontier<IT>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                    std::atomic<bool>& found_augmenting_path,
                    std::atomic<IT> &masterTID,
                    std::atomic<IT> &num_contracting_blossoms);
    template <typename IT, typename VT>
    static void next_iteration(Graph<IT, VT>& graph, 
                    std::atomic<IT> & currentRoot,
                    std::atomic<IT> & num_enqueued,  
                    int tid,                  
                    //moodycamel::ConcurrentQueue<IT> &worklist,
                    std::vector<moodycamel::ConcurrentQueue<IT, moodycamel::ConcurrentQueueDefaultTraits>> &worklists);
    template <typename IT, typename VT>
    static Vertex<IT> * search_persistent(Graph<IT, VT>& graph, 
                    IT & V_index,
                    Frontier<IT> & f,
                    moodycamel::ConcurrentQueue<IT> &worklist,
                    int tid,
                    std::atomic<bool> & found_augmenting_path);
    template <typename IT, typename VT>
    static void augment(Graph<IT, VT>& graph, 
                    Vertex<IT> * TailOfAugmentingPath,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::vector<IT> & path);
    template <typename IT, typename VT>
    static void extract_path(Graph<IT, VT>& graph, 
                    Vertex<IT> * TailOfAugmentingPath,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::vector<IT> & path);
    template <typename IT, typename VT>
    static void pathThroughBlossom(Graph<IT, VT>& graph, 
                        // V
                        const Vertex<IT> * TailOfAugmentingPath,
                        const Vertex<IT> * TailOfAugmentingPathBase,
                        std::vector<Vertex<IT>> & vertexVector,
                        //std::list<IT> & path,
                        std::vector<IT> & path);
    template <typename IT, typename VT>
    static void pathThroughBlossomConcurrent(Graph<IT, VT>& graph, 
                        // V
                        const Vertex<IT> * TailOfAugmentingPath,
                        const Vertex<IT> * TailOfAugmentingPathBase,
                        std::vector<Vertex<IT>> & vertexVector,
                        //std::list<IT> & path,
                        std::vector<IT> & path);
};

template <typename IT, typename VT>
void Matcher::match(Graph<IT, VT>& graph) {
    std::vector<Vertex<IT>> vertexVector;
    Vertex<IT> * TailOfAugmentingPath;
    Frontier<IT> f;
    std::vector<IT> path;
    IT i;
    const size_t N = graph.getN();
    auto allocate_start = high_resolution_clock::now();
    vertexVector.reserve(graph.getN());
    path.reserve(graph.getN());
    std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
    auto allocate_end = high_resolution_clock::now();
    auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
    std::cout << "Vertex Vector (11|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
    // Access the graph elements as needed
    auto search_start = high_resolution_clock::now();
    //for (; (i=currentRoot++) < N;) {
    for (i = 0; i < N; ++i) {
        //currentRoot.store(i,std::memory_order_release);
        if (graph.IsMatched(i))
            continue;
        else{
            search(graph,i,f,vertexVector);
            if (f.TailOfAugmentingPathVertexIndex!=-1){
                    TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                    augment(graph,TailOfAugmentingPath,vertexVector,path);
                    f.reinit(vertexVector);
                    path.clear();
                    f.clear();
                //printf("FOUND AP!\n");
            } else {
                f.clear();
                //printf("DIDNT FOUND AP!\n");
            }
        }
    }
    auto search_end = high_resolution_clock::now();
    auto duration_search = duration_cast<milliseconds>(search_end - search_start);
    std::cout << "Algorithm execution time: "<< (1.0*duration_search.count())/1000.0 << " seconds" << '\n';
}


template <typename IT, typename VT>
void Matcher::match(Graph<IT, VT>& graph, Statistics<IT>& stats) {

        std::vector<Vertex<IT>> vertexVector;
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        Vertex<IT> * TailOfAugmentingPath;
        Frontier<IT> f;
        std::vector<IT> path;
    // Access the graph elements as needed
    for (std::size_t i = 0; i < graph.getN(); ++i) {
        if (!graph.IsMatched(i)) {
            //printf("SEARCHING FROM %ld!\n",i);
            // Your matching logic goes here...
            auto search_start = high_resolution_clock::now();
            TailOfAugmentingPath=search(graph,i,f);
            auto search_end = high_resolution_clock::now();
            // If not a nullptr, I found an AP.
            if (TailOfAugmentingPath){
                augment(graph,TailOfAugmentingPath,vertexVector,path);
                stats.write_entry(path.size() ? (2*path.size()-1):0,f.tree.size(),duration_cast<microseconds>(search_end - search_start));
                f.reinit(vertexVector);
                path.clear();
                f.clear();
                //printf("FOUND AP!\n");
            } else {
                stats.write_entry(f.path.size() ? (2*f.path.size()-1):0,f.tree.size(),duration_cast<microseconds>(search_end - search_start));
                f.clear();
                //printf("DIDNT FOUND AP!\n");
            }
        }
    }
}

#include "ThreadFactory.h"
template <typename IT, typename VT>
void Matcher::match_wl(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold) {
    auto mt_thread_coordination_start = high_resolution_clock::now();
    size_t capacity = 1;
    std::vector<moodycamel::ConcurrentQueue<Frontier<IT,std::deque>, moodycamel::ConcurrentQueueDefaultTraits>> worklists;
    worklists.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        // Initialize each queue with the desired parameters
        worklists.emplace_back(moodycamel::ConcurrentQueue<Frontier<IT,std::deque>>{capacity});
    }
    
    std::vector<std::mutex> worklistMutexes(num_threads);
    std::vector<std::condition_variable> worklistCVs(num_threads);

    std::atomic<IT> num_enqueued(0);
    std::atomic<IT> num_dequeued(0);
    std::atomic<IT> num_contracting_blossoms(0);
    std::atomic<IT> currentRoot(-1);
    std::atomic<IT> masterTID(-1);
    std::atomic<bool> found_augmenting_path(false);
    
    //std::vector<std::atomic<bool>> atomicBoolVector(num_threads);
    std::vector<std::thread> workers(num_threads);
    std::vector<size_t> read_messages;
    read_messages.resize(num_threads);
    auto mt_thread_coordination_end = high_resolution_clock::now();
    auto durationmt = duration_cast<microseconds>(mt_thread_coordination_end - mt_thread_coordination_start);
    std::cout << "Worklist and atomic variable allocation time: "<< durationmt.count() << " microseconds" << '\n';



    //spinning.resize(num_threads,false);
    // Access the graph elements as needed
    ThreadFactory::create_threads_concurrentqueue_wl<IT,VT>(workers, num_threads,read_messages,
    worklists,masterTID,graph,
    currentRoot,found_augmenting_path,
    worklistMutexes,worklistCVs,num_enqueued,num_dequeued,num_contracting_blossoms,
    deferral_threshold);

    auto join_thread_start = high_resolution_clock::now();
    for (auto& t : workers) {
        t.join();
    }
    auto join_thread_end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(join_thread_end - join_thread_start);
    std::cout << "Thread joining time: "<< duration.count() << " seconds" << '\n';

    std::cout << "NUM ENQUEUED " << num_enqueued.load() << '\n';
    std::cout << "NUM DEQUEUED " << num_dequeued.load() << '\n';
    std::cout << "NUM SPINNING " << num_contracting_blossoms.load() << '\n';

    size_t tot_read_messages = 0;
    int ni = 0;
    for (size_t n : read_messages){
        printf("reader: \t\t %d read_messages %zu \n", ni++,n);
        tot_read_messages += n;
    }
    printf("Total read_messages %zu \n", tot_read_messages);
}



template <typename IT, typename VT>
void Matcher::match_wl2(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold) {
    auto mt_thread_coordination_start = high_resolution_clock::now();
    size_t capacity = 100;
    moodycamel::ConcurrentQueue<IT> worklist{capacity};
    std::atomic<IT> currentRoot(-1);
    std::atomic<IT> masterTID(-1);
    std::atomic<bool> finished(false);
    std::vector<size_t> read_messages;
    std::vector<std::thread> workers(num_threads);
    std::vector<std::atomic_flag> dead(graph.getN()); 
    read_messages.resize(num_threads);
    auto mt_thread_coordination_end = high_resolution_clock::now();
    auto durationmt = duration_cast<microseconds>(mt_thread_coordination_end - mt_thread_coordination_start);
    std::cout << "Worklist and atomic variable allocation time: "<< durationmt.count() << " microseconds" << '\n';

    //spinning.resize(num_threads,false);
    // Access the graph elements as needed
    ThreadFactory::create_threads_concurrentqueue_wl2<IT,VT>(workers, num_threads,read_messages,
    worklist,dead,finished,
    masterTID,graph,
    currentRoot,
    deferral_threshold);

    auto join_thread_start = high_resolution_clock::now();
    for (auto& t : workers) {
        t.join();
    }
    auto join_thread_end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(join_thread_end - join_thread_start);
    std::cout << "Thread joining time: "<< duration.count() << " seconds" << '\n';

    size_t tot_read_messages = 0;
    int ni = 0;
    for (size_t n : read_messages){
        printf("reader: \t\t %d read_messages %zu \n", ni++,n);
        tot_read_messages += n;
    }
    printf("Total read_messages %zu \n", tot_read_messages);
}


#include "ThreadFactory.h"
template <typename IT, typename VT>
void Matcher::match_wl3(Graph<IT, VT>& graph, 
                        int num_threads,
                        int deferral_threshold) {
    auto mt_thread_coordination_start = high_resolution_clock::now();
    size_t capacity = 1;
    std::vector<moodycamel::ConcurrentQueue<Frontier<IT,std::deque>, moodycamel::ConcurrentQueueDefaultTraits>> worklists;
    worklists.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        // Initialize each queue with the desired parameters
        worklists.emplace_back(moodycamel::ConcurrentQueue<Frontier<IT,std::deque>>{capacity});
    }
    
    std::vector<std::mutex> worklistMutexes(num_threads);
    std::vector<std::condition_variable> worklistCVs(num_threads);

    std::atomic<IT> num_enqueued(0);
    std::atomic<IT> num_dequeued(0);
    std::atomic<IT> num_contracting_blossoms(0);
    std::atomic<IT> currentRoot(-1);
    std::atomic<IT> masterTID(-1);
    std::atomic<bool> found_augmenting_path(false);
    
    //std::vector<std::atomic<bool>> atomicBoolVector(num_threads);
    std::vector<std::thread> workers(num_threads);
    std::vector<size_t> read_messages;
    read_messages.resize(num_threads);
    auto mt_thread_coordination_end = high_resolution_clock::now();
    auto durationmt = duration_cast<microseconds>(mt_thread_coordination_end - mt_thread_coordination_start);
    std::cout << "Worklist and atomic variable allocation time: "<< durationmt.count() << " microseconds" << '\n';



    //spinning.resize(num_threads,false);
    // Access the graph elements as needed
    ThreadFactory::create_threads_concurrentqueue_wl3<IT,VT>(workers, num_threads,read_messages,
    worklists,masterTID,graph,
    currentRoot,found_augmenting_path,
    worklistMutexes,worklistCVs,num_enqueued,num_dequeued,num_contracting_blossoms,
    deferral_threshold);

    auto join_thread_start = high_resolution_clock::now();
    for (auto& t : workers) {
        t.join();
    }
    auto join_thread_end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(join_thread_end - join_thread_start);
    std::cout << "Thread joining time: "<< duration.count() << " seconds" << '\n';

    std::cout << "NUM ENQUEUED " << num_enqueued.load() << '\n';
    std::cout << "NUM DEQUEUED " << num_dequeued.load() << '\n';
    std::cout << "NUM SPINNING " << num_contracting_blossoms.load() << '\n';

    size_t tot_read_messages = 0;
    int ni = 0;
    for (size_t n : read_messages){
        printf("reader: \t\t %d read_messages %zu \n", ni++,n);
        tot_read_messages += n;
    }
    printf("Total read_messages %zu \n", tot_read_messages);
}




template <typename IT, typename VT>
void Matcher::match_persistent_wl(Graph<IT, VT>& graph,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                bool &finished) {

        std::vector<Vertex<IT>> vertexVector;
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        Vertex<IT> * TailOfAugmentingPath;
        Frontier<IT> f;
        std::vector<IT> path;
    // Access the graph elements as needed
    for (std::size_t i = 0; i < graph.getN(); ++i) {
        if (!graph.IsMatched(i)) {
            //printf("SEARCHING FROM %ld!\n",i);
            // Your matching logic goes here...
            auto search_start = high_resolution_clock::now();
            TailOfAugmentingPath=search(graph,i,f);
            auto search_end = high_resolution_clock::now();
            // If not a nullptr, I found an AP.
            if (TailOfAugmentingPath){
                augment(graph,TailOfAugmentingPath,vertexVector,path);
                f.reinit(vertexVector);
                path.clear();
                f.clear();
                //printf("FOUND AP!\n");
            } else {
                //stats.write_entry(f.path.size() ? (2*f.path.size()-1):0,f.tree.size(),duration_cast<microseconds>(search_end - search_start));
                f.clear();
                //printf("DIDNT FOUND AP!\n");
            }
        }
    }
}

template <typename IT, typename VT>
void Matcher::match_persistent_wl4(Graph<IT, VT>& graph,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<bool>& found_augmenting_path,
                                std::atomic<IT> & currentRoot,
                                std::vector<std::mutex> &worklistMutexes,
                                std::vector<std::condition_variable> &worklistCVs,
                                int tid) {
    std::vector<Vertex<IT>> vertexVector;
    Vertex<IT> * TailOfAugmentingPath;
    std::vector<IT> path;
    Frontier<IT> f;
    IT N = graph.getN();
    bool valid = true;
    // first thread to reach here claims master status.
    auto thread_match_start = high_resolution_clock::now();
    auto allocate_start = high_resolution_clock::now();
    vertexVector.reserve(graph.getN());
    std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
    auto allocate_end = high_resolution_clock::now();
    auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
    std::cout << "TID(" << tid << ") Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
    IT local_root;
    auto search_start = high_resolution_clock::now();
    for (;(local_root=++currentRoot) < N;) {
        read_messages[tid]++;
        while(!graph.IsMatched(local_root)) {
            vertexVector[local_root].AgeField=f.time++;
            f.tree.push_back(vertexVector[local_root]);
            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,local_root,f.stack);
            
            // If returned without an error stemming from
            // someone else augmenting whilst I am searching
            // Check if I found an AP.
            if(concurrent_search(graph,f,vertexVector)){
                // Successfuly found AP. try to match
                if(f.TailOfAugmentingPathVertexIndex!=-1){
                    TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                    extract_path(graph,TailOfAugmentingPath,vertexVector,path);
                    worklistMutexes[0].lock();
                    valid = true;
                    for (auto E : path) {
                        //Match(EdgeFrom(E)) = E;
                        valid &= graph.GetMatchField(Graph<IT,VT>::EdgeFrom(graph,E))==vertexVector[Graph<IT,VT>::EdgeFrom(graph,E)].MatchField;
                        //Match(EdgeTo(E)) = E;
                        valid &= graph.GetMatchField(Graph<IT,VT>::EdgeTo(graph,E))==vertexVector[Graph<IT,VT>::EdgeTo(graph,E)].MatchField;
                    }
                    if (valid){
                        for (auto E : path) {
                            //Match(EdgeFrom(E)) = E;
                            graph.SetMatchField(Graph<IT,VT>::EdgeFrom(graph,E),E);
                            //Match(EdgeTo(E)) = E;
                            graph.SetMatchField(Graph<IT,VT>::EdgeTo(graph,E),E);
                        }
                    }
                    worklistMutexes[0].unlock();
                    //augment(graph,TailOfAugmentingPath,vertexVector,path);
                } else {
                    valid = true;
                }
            // Concurrent search failed due to augmentation problems.
            } else {
                valid = false;
            }
            f.reinit(vertexVector);
            f.clear();
            path.clear();
            if (!valid){
                continue;
            } else {
                break;
            }
        }
    }
    auto search_end = high_resolution_clock::now();
    auto duration_search = duration_cast<seconds>(search_end - search_start);
    std::cout << "Thread "<< tid << " algorithm execution time: "<< duration_search.count() << " seconds" << '\n';
}


template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
void Matcher::match_persistent_wl5(Graph<IT, VT>& graph,
                                std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<bool>& found_augmenting_path,
                                std::atomic<IT> & currentRoot,
                                std::vector<std::mutex> &worklistMutexes,
                                std::vector<std::condition_variable> &worklistCVs,
                                int tid,
                                std::atomic<IT> & num_enqueued,
                                std::atomic<IT> & num_dequeued,
                                std::atomic<IT> & num_contracting_blossoms,
                                int deferral_threshold) {
    std::vector<Vertex<IT>> vertexVector;
    Vertex<IT> * TailOfAugmentingPath;
    std::vector<IT> path;
    Frontier<IT,std::deque> f;
    IT N = graph.getN();
    const size_t nworkers = worklists.size();
    bool valid = true;
    // first thread to reach here claims master status.
    auto thread_match_start = high_resolution_clock::now();
    auto allocate_start = high_resolution_clock::now();
    vertexVector.reserve(graph.getN());
    std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
    auto allocate_end = high_resolution_clock::now();
    auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
    std::cout << "TID(" << tid << ") Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';

    IT expected = -1;
    IT desired = 0;
    IT local_root;
    // Master
    auto search_start = high_resolution_clock::now();
    if (currentRoot.compare_exchange_strong(expected, desired)) {
        std::cout << "MASTER TID(" << tid << ")" << '\n';
        for (; (local_root=currentRoot++) < N;) {
            read_messages[tid]++;
            if (!graph.IsMatched(local_root)) {
                //printf("SEARCHING FROM %ld!\n",i);
                // Your matching logic goes here...
                search(graph,local_root,f,vertexVector);
                // If not -1, I found an AP.
                if (f.TailOfAugmentingPathVertexIndex!=-1){
                        TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                        augment(graph,TailOfAugmentingPath,vertexVector,path);
                        f.reinit(vertexVector);
                        path.clear();
                        f.clear();
                    //printf("FOUND AP!\n");
                } else {
                    f.clear();
                    //printf("DIDNT FOUND AP!\n");
                }
            }
        }
        for (auto & cv:worklistCVs)
            cv.notify_one();
    } else {
        // If the worklist is empty, wait for a signal
        while(currentRoot.load(std::memory_order_relaxed)<N){
            std::unique_lock<std::mutex> lock(worklistMutexes[tid]);
            worklistCVs[tid].wait(lock, [&] { return worklists[tid].size_approx() || currentRoot.load(std::memory_order_relaxed)>=N; });
            if (worklists[tid].try_dequeue(f)){
                continue_search(graph,f,vertexVector);
            }
        }
    }
    auto search_end = high_resolution_clock::now();
    auto duration_search = duration_cast<seconds>(search_end - search_start);
    std::cout << "Thread "<< tid << " algorithm execution time: "<< duration_search.count() << " seconds" << '\n';
}



template <typename IT, typename VT>
void Matcher::match_persistent_wl6(Graph<IT, VT>& graph,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                std::vector<std::atomic_flag> &dead,
                                std::atomic<bool> &finished,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<IT> & currentRoot,
                                int tid,
                                int deferral_threshold) {
    IT expected = -1;
    IT desired = -0;
    std::vector<Vertex<IT>> vertexVector;
    Vertex<IT> * TailOfAugmentingPath;
    Frontier<IT> f;
    std::vector<IT> path;
    std::vector<IT> deferred_roots;
    IT i;
    const size_t N = graph.getN();
    // First to encounter this code will see currentRoot == -1,
    // it will be atomically exchanged with 0, and return true.
    // All others will modify expected, inconsequentially,
    // and enter the while loop.
    auto thread_match_start = high_resolution_clock::now();
    if (currentRoot.compare_exchange_strong(expected, desired)) {
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        // Access the graph elements as needed
        auto search_start = high_resolution_clock::now();
        for (i = 0; i < N; ++i) {
            if (!graph.IsMatched(i)) {
                //printf("SEARCHING FROM %ld!\n",i);
                // Your matching logic goes here...
                bool finished = search(graph,i,f,vertexVector,deferral_threshold);
                // If not -1, I found an AP.
                if(finished){
                    if (f.TailOfAugmentingPathVertexIndex!=-1){
                            TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                            augment(graph,TailOfAugmentingPath,vertexVector,path);
                            f.reinit(vertexVector);
                            path.clear();
                            f.clear();
                        //printf("FOUND AP!\n");
                    } else {
                        f.clear();
                        //printf("DIDNT FOUND AP!\n");
                    }
                } else {
                    deferred_roots.push_back(i);
                    worklist.enqueue(i);
                    f.reinit(vertexVector);
                    f.clear();
                }
            }
        }
        while(deferred_roots.size()){
            i = deferred_roots.back();
            deferred_roots.pop_back();
            if (dead[i].test() || graph.IsMatched(i))
                continue;
            else{
                search(graph,i,f,vertexVector);
                if (f.TailOfAugmentingPathVertexIndex!=-1){
                        TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                        augment(graph,TailOfAugmentingPath,vertexVector,path);
                        f.reinit(vertexVector);
                        path.clear();
                        f.clear();
                    //printf("FOUND AP!\n");
                } else {
                    read_messages[tid]++;       
                    f.clear();
                    //printf("DIDNT FOUND AP!\n");
                }
            }
        }
        finished.store(true);
        auto search_end = high_resolution_clock::now();
        auto duration_search = duration_cast<seconds>(search_end - search_start);
        std::cout << "Algorithm execution time: "<< duration_search.count() << " seconds" << '\n';
    } else {
        // If the worklist is empty, wait for a signal
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        while(!finished.load(std::memory_order_relaxed)){
            IT local_root;
            if (worklist.try_dequeue(local_root)){
                if (dead[local_root].test() || graph.IsMatched(local_root))
                    continue;
                vertexVector[local_root].AgeField=f.time++;
                f.tree.push_back(vertexVector[local_root]);
                Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,local_root,f.stack);
                
                // If returned without an error stemming from
                // someone else augmenting whilst I am searching
                // Check if I found an AP.
                if(concurrent_search(graph,f,vertexVector)){
                    // Successfuly found AP. try to match
                    if(f.TailOfAugmentingPathVertexIndex!=-1){

                    } else {
                        dead[local_root].test_and_set();;
                        read_messages[tid]++;
                    }
                // Concurrent search failed due to augmentation problems.
                }
                f.reinit(vertexVector);
                f.clear();
            }
        }
    }

}


template <typename IT, typename VT>
void Matcher::match_persistent_wl7(Graph<IT, VT>& graph,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                std::vector<std::atomic_flag> &dead,
                                std::atomic<bool> &finished,
                                std::atomic<IT> &masterTID,
                                std::vector<size_t> &read_messages,
                                std::atomic<IT> & currentRoot,
                                int tid,
                                int deferral_threshold) {
    IT expected = -1;
    IT desired = -0;
    std::vector<Vertex<IT>> vertexVector;
    Vertex<IT> * TailOfAugmentingPath;
    Frontier<IT> f;
    std::vector<IT> path;
    IT i;
    const size_t N = graph.getN();
    // First to encounter this code will see currentRoot == -1,
    // it will be atomically exchanged with 0, and return true.
    // All others will modify expected, inconsequentially,
    // and enter the while loop.
    auto thread_match_start = high_resolution_clock::now();
    if (currentRoot.compare_exchange_strong(expected, desired)) {
        std::cout << "MASTER TID(" << tid << ")" << '\n';
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        path.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "TID(" << tid << ") Vertex Vector (11|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        // Access the graph elements as needed
        auto search_start = high_resolution_clock::now();
        //for (; (i=currentRoot++) < N;) {
        for (i = 0; i < N; ++i) {
            //currentRoot.store(i,std::memory_order_release);
            if (dead[i].test() || graph.IsMatched(i))
                continue;
            else{
                currentRoot.store(i,std::memory_order_relaxed);
                search(graph,i,f,vertexVector);
                if (f.TailOfAugmentingPathVertexIndex!=-1){
                        TailOfAugmentingPath=&vertexVector[f.TailOfAugmentingPathVertexIndex];
                        augment(graph,TailOfAugmentingPath,vertexVector,path);
                        f.reinit(vertexVector);
                        path.clear();
                        f.clear();
                    //printf("FOUND AP!\n");
                } else {
                    if(!dead[i].test_and_set())
                        read_messages[tid]++;
                    f.clear();
                    //printf("DIDNT FOUND AP!\n");
                }
            }
        }
        finished.store(true);
        auto search_end = high_resolution_clock::now();
        auto duration_search = duration_cast<milliseconds>(search_end - search_start);
        std::cout << "Algorithm execution time: "<< (1.0*duration_search.count())/1000.0 << " seconds" << '\n';
    } else {
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "TID(" << tid << ") Vertex Vector (10|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        while(!finished.load(std::memory_order_relaxed)){
            IT lower_bound = currentRoot.load(std::memory_order_relaxed);
            
            //IT random_vertex = rand()%(N-lower_bound + 1) + lower_bound;
            IT random_vertex = lower_bound+100+tid;
            if (random_vertex >= N || dead[random_vertex].test() || graph.IsMatched(random_vertex))
                continue;
            vertexVector[random_vertex].AgeField=f.time++;
            f.tree.push_back(vertexVector[random_vertex]);
            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,random_vertex,f.stack);
            
            // If returned without an error stemming from
            // someone else augmenting whilst I am searching
            // Check if I found an AP.
            if(concurrent_search(graph,f,vertexVector)){
                // Successfuly found AP. try to match
                if(f.TailOfAugmentingPathVertexIndex!=-1){

                } else {
                    if(!dead[random_vertex].test_and_set())
                        read_messages[tid]++;
                }
            // Concurrent search failed due to augmentation problems.
            }
            f.reinit(vertexVector);
            f.clear();   
        }
    }

}


template <typename IT, typename VT>
void Matcher::match_persistent_wl2(Graph<IT, VT>& graph,
                                std::vector<moodycamel::ConcurrentQueue<IT, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                moodycamel::ConcurrentQueue<IT> &worklist,
                                std::vector<size_t> &read_messages,
                                std::atomic<bool>& found_augmenting_path,
                                std::atomic<IT> & currentRoot,
                                std::vector<std::mutex> &worklistMutexes,
                                std::vector<std::condition_variable> &worklistCVs,
                                int tid,
                                std::atomic<IT> & num_enqueued,
                                std::atomic<IT> & num_dequeued,
                                std::atomic<IT> & num_contracting_blossoms) {


        std::vector<Vertex<IT>> vertexVector;
        auto allocate_start = high_resolution_clock::now();
        vertexVector.reserve(graph.getN());
        std::iota(vertexVector.begin(), vertexVector.begin()+graph.getN(), 0);
        auto allocate_end = high_resolution_clock::now();
        auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
        std::cout << "Vertex Vector (9|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
        Vertex<IT> * TailOfAugmentingPath;
        Frontier<IT> f;
        std::vector<IT> path;

    const size_t N = graph.getN();
    //const size_t N = 5;
    IT V_index;

    IT expected = -2;
    IT desired = -1;
    // First to encounter this code will see currentRoot == -2,
    // it will be atomically exchanged with -1, and return true.
    // All others will modify expected, inconsequentially,
    // and enter the while loop.
    // The worker thread has done the work,
    // Notify the master thread to continue the work.
    auto thread_match_start = high_resolution_clock::now();
    if (currentRoot.compare_exchange_strong(expected, desired)) {
        next_iteration(graph,currentRoot,num_enqueued,tid,worklists);
    }
    // finished_algorithm when currentRoot == N
    while(currentRoot.load(std::memory_order_relaxed)!=N){
        if (worklists[tid].try_dequeue(V_index)){
            read_messages[tid]++;       

            // Turn on flag
            TailOfAugmentingPath = search_persistent(graph,V_index,f,worklist,tid,
            found_augmenting_path);

            // At all-spin state, there should be parity between 
            // num en/dequeue
            num_dequeued++;

            // All parallel searchers have at least gotten here.
            // It is possible that one
            // Always false
            bool expected = false;
            // Found AP
            bool desired = TailOfAugmentingPath != nullptr;
            // First searcher to find an AP
            // If any AP was found only one walker will enter this loop
            if (desired && found_augmenting_path.compare_exchange_strong(expected, desired)) {
                // Need to wait here to avoid augmenting while the graph is being traversed.
                while(num_dequeued.load()!=num_enqueued.load()){}
                augment(graph,TailOfAugmentingPath,f);
            }

            //num_contracting_blossoms++;
            if (1+num_contracting_blossoms.fetch_add(1) == num_dequeued.load() &&
                num_dequeued.load() == num_enqueued.load()) {
                found_augmenting_path.store(false);
                next_iteration(graph,currentRoot,num_enqueued,tid,worklists);
            }

            f.reinit();
            f.clear();

        } else {
            // If the worklist is empty, wait for a signal
            std::unique_lock<std::mutex> lock(worklistMutexes[tid]);
            worklistCVs[tid].wait(lock, [&] { return worklists[tid].size_approx() || currentRoot.load(std::memory_order_relaxed)==N; });
        }
    }
    auto thread_match_end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(thread_match_end - thread_match_start);
    std::cout << "Thread "<< tid << " algorithm execution time: "<< duration.count() << " seconds" << '\n';

    for (auto & cv:worklistCVs)
        cv.notify_one();

}

template <typename IT, typename VT>
void Matcher::next_iteration(Graph<IT, VT>& graph, 
                    std::atomic<IT> & currentRoot,
                    std::atomic<IT> & num_enqueued,
                    //moodycamel::ConcurrentQueue<IT> &worklist,
                    int tid,
                    std::vector<moodycamel::ConcurrentQueue<IT, moodycamel::ConcurrentQueueDefaultTraits>> &worklists
                    ){
    while(++currentRoot < graph.getN()){
        if (!graph.IsMatched(currentRoot)) {
            //printf("Enqueuing %d\n",i);
            num_enqueued++;
            worklists[tid].enqueue(currentRoot);
            break;
        }
        // Rest of pushes are done by the persistent threads.
    }
    // Turn off flag
    //finished_algorithm.store(currentRoot==graph.getN(),std::memory_order_relaxed);
}

template <typename IT, typename VT>
Vertex<IT> * Matcher::search_persistent(Graph<IT, VT>& graph, 
                    IT & V_index,
                    Frontier<IT> & f,
                    moodycamel::ConcurrentQueue<IT> &worklist,
                    int tid,
                    std::atomic<bool> & found_augmenting_path) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT time = 0;
    std::vector<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    DisjointSetUnion<IT> &dsu = f.dsu;
    std::vector<Vertex<IT>> & vertexVector = f.vertexVector;

    nextVertex = &vertexVector[V_index];
    nextVertex->AgeField=time++;
    tree.push_back(*nextVertex);

    // Push edges onto stack, breaking if that stackEdge is a solution.
    Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,V_index,stack);
    // Gracefully exit other searchers if an augmenting path is found.
    while(!stack.empty() && !found_augmenting_path.load(std::memory_order_relaxed)){
        stackEdge = stack.back();
        stack.pop_back();

        #ifndef NDEBUG
        FromBaseVertexID = dsu[Graph<IT,VT>::EdgeFrom(graph,stackEdge)];
        auto FromBaseVertexIDTest = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        assert(FromBaseVertexID==FromBaseVertexIDTest);
        #else
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        #endif
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        #ifndef NDEBUG
        ToBaseVertexID = dsu[Graph<IT,VT>::EdgeTo(graph,stackEdge)];
        auto ToBaseVertexIDTest = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  
        assert(ToBaseVertexID==ToBaseVertexIDTest);
        #else
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  
        #endif
        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            // I'll let the augment path method recover the path.
            return ToBase;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            // Not sure if this is wrong or the augment method is wrong
            Blossom::Shrink(graph,stackEdge,dsu,vertexVector,stack);
        }
    }
    return nullptr;
}

template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
void Matcher::search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT &time = f.time;
    StackType<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    //auto inserted = vertexMap.try_emplace(V_index,Vertex<IT>(time++,Label::EvenLabel));
    nextVertex = &vertexVector[V_index];
    nextVertex->AgeField=time++;
    tree.push_back(*nextVertex);

    // Push edges onto stack, breaking if that stackEdge is a solution.
    Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,V_index,stack);
    while(!stack.empty()){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  

        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            // Minimize atomic matching access
            /*
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);*/
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            ToBase->MatchField=matchedEdge;
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            nextVertex->MatchField=matchedEdge;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            // Not sure if this is wrong or the augment method is wrong
            // Minimize atomic matching access
            //Blossom::Shrink(graph,stackEdge,vertexVector,stack);
            Blossom::Shrink_Concurrent(graph,stackEdge,vertexVector,stack);

        }
    }
    return;
}


template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
bool Matcher::search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    IT deferral_threshold) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT &time = f.time;
    StackType<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    //auto inserted = vertexMap.try_emplace(V_index,Vertex<IT>(time++,Label::EvenLabel));
    nextVertex = &vertexVector[V_index];
    nextVertex->AgeField=time++;
    tree.push_back(*nextVertex);

    // Push edges onto stack, breaking if that stackEdge is a solution.
    Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,V_index,stack);
    while(!stack.empty()){
        if (stack.size()>deferral_threshold)
            return false;
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  

        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return true;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            // Minimize atomic matching access
            /*
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);*/
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            ToBase->MatchField=matchedEdge;
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            nextVertex->MatchField=matchedEdge;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            // Not sure if this is wrong or the augment method is wrong
            // Minimize atomic matching access
            //Blossom::Shrink(graph,stackEdge,vertexVector,stack);
            Blossom::Shrink_Concurrent(graph,stackEdge,vertexVector,stack);

        }
    }
    return true;
}


template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
void Matcher::continue_search(Graph<IT, VT>& graph, 
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT &time = f.time;
    StackType<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    //auto inserted = vertexMap.try_emplace(V_index,Vertex<IT>(time++,Label::EvenLabel));

    while(!stack.empty()){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  

        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            // Minimize atomic matching access
            /*
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);*/
            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            ToBase->MatchField=matchedEdge;
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            nextVertex->MatchField=matchedEdge;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            // Not sure if this is wrong or the augment method is wrong
            // Minimize atomic matching access
            //Blossom::Shrink(graph,stackEdge,vertexVector,stack);
            Blossom::Shrink_Concurrent(graph,stackEdge,vertexVector,stack);

        }
    }
    return;
}



template <typename IT, typename VT>
bool Matcher::capped_search(Graph<IT, VT>& graph, 
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    int max_depth) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;

    IT &time = f.time;
    std::vector<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    while(!stack.empty()){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  
        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return false;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            Blossom::Shrink(graph,stackEdge,vertexVector,stack);
        }

        if (stack.size()>max_depth)
            return true;
    }
    return false;
}

template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
bool Matcher::concurrent_search(Graph<IT, VT>& graph, 
                    FrontierType<IT, StackType> & f,
                    std::vector<Vertex<IT>> & vertexVector) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;

    IT &time = f.time;
    StackType<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    while(!stack.empty() ){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  
        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return true;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            ToBase->MatchField=matchedEdge;
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            if(nextVertex->AgeField!=-1)
                return false;
            nextVertex->AgeField=time++;
            nextVertex->MatchField=matchedEdge;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Handle mysterious error by trying again.
            if(!Blossom::Shrink_Concurrent(graph,stackEdge,vertexVector,stack))
                return false;
        }

    }
    return true;
}


template <typename IT, typename VT>
void Matcher::start_search(Graph<IT, VT>& graph, 
                    const size_t V_index,
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::atomic<IT> & num_enqueued,
                    std::vector<moodycamel::ConcurrentQueue<Frontier<IT>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                    std::atomic<bool>& found_augmenting_path,
                    std::atomic<IT> &masterTID) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT &time = f.time;
    std::vector<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    //auto inserted = vertexMap.try_emplace(V_index,Vertex<IT>(time++,Label::EvenLabel));
    nextVertex = &vertexVector[V_index];
    nextVertex->AgeField=time++;
    tree.push_back(*nextVertex);

    // Push edges onto stack, breaking if that stackEdge is a solution.
    Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,V_index,stack);
    while(!stack.empty()){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  

        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            Blossom::Shrink(graph,stackEdge,vertexVector,stack);
        }
    }
    return;
}

template <typename IT, typename VT>
void Matcher::continue_search(Graph<IT, VT>& graph, 
                    Frontier<IT> & f,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::atomic<IT> & num_enqueued,
                    std::vector<moodycamel::ConcurrentQueue<Frontier<IT>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                    std::atomic<bool>& found_augmenting_path,
                    std::atomic<IT> &masterTID,
                    std::atomic<IT> &num_contracting_blossoms) {
    Vertex<IT> *FromBase,*ToBase, *nextVertex;
    IT FromBaseVertexID,ToBaseVertexID;
    IT stackEdge, matchedEdge;
    IT nextVertexIndex;
    IT &time = f.time;
    std::vector<IT> &stack = f.stack;
    std::vector<Vertex<IT>> &tree = f.tree;
    while(!stack.empty() && !found_augmenting_path.load(std::memory_order_release)){
        stackEdge = stack.back();
        stack.pop_back();
        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..

        FromBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeFrom(graph,stackEdge),vertexVector);  
        FromBase = &vertexVector[FromBaseVertexID];

        // Necessary because vertices dont know their own index.
        // It simplifies vector creation..
        ToBaseVertexID = DisjointSetUnionHelper<IT>::getBase(Graph<IT,VT>::EdgeTo(graph,stackEdge),vertexVector);  
        ToBase = &vertexVector[ToBaseVertexID];

        // Edge is between two vertices in the same blossom, continue.
        if (FromBase == ToBase)
            continue;
        if(!FromBase->IsEven()){
            std::swap(FromBase,ToBase);
            std::swap(FromBaseVertexID,ToBaseVertexID);
        }
        // An unreached, unmatched vertex is found, AN AUGMENTING PATH!
        if (!ToBase->IsReached() && !graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);
            //graph.SetMatchField(ToBaseVertexID,stackEdge);
            // I'll let the augment path method recover the path.
            f.TailOfAugmentingPathVertexIndex=ToBase->LabelField;
            return;
        } else if (!ToBase->IsReached() && graph.IsMatched(ToBaseVertexID)){
            ToBase->TreeField=stackEdge;
            ToBase->AgeField=time++;
            tree.push_back(*ToBase);

            matchedEdge=graph.GetMatchField(ToBaseVertexID);
            nextVertexIndex = Graph<IT,VT>::Other(graph,matchedEdge,ToBaseVertexID);
            nextVertex = &vertexVector[nextVertexIndex];
            nextVertex->AgeField=time++;
            tree.push_back(*nextVertex);

            Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,nextVertexIndex,stack,matchedEdge);

        } else if (ToBase->IsEven()) {
            // Shrink Blossoms
            Blossom::Shrink(graph,stackEdge,vertexVector,stack);
        }
    }
    return;
}

template <typename IT, typename VT>
void Matcher::augment(Graph<IT, VT>& graph, 
                    Vertex<IT> * TailOfAugmentingPath,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::vector<IT> & path) {

    IT edge;
    // W
    Vertex<IT>*nextVertex;
    Vertex<IT>*nextVertexBase;
    do
    {
        //ListPut(Tree(V), P);
        edge = TailOfAugmentingPath->TreeField;
        path.push_back(edge);

        //W = Other(Tree(V), V);
        ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
        auto nextVertexID = Graph<IT,VT>::Other(graph,edge,TailOfAugmentingPath_VertexID);
        nextVertex = &vertexVector[nextVertexID];

        //B = Base(Blossom(W));
        // GJS
        auto nextVertexBaseID = DisjointSetUnionHelper<IT>::getBase(nextVertexID,vertexVector);  

        nextVertexBase = &vertexVector[nextVertexBaseID];
        
        // Path(W, B, P);
        pathThroughBlossom(graph,nextVertex,nextVertexBase,vertexVector,path);

        //V = Other(Match(B), B);
        ptrdiff_t nextVertexBase_VertexID = nextVertexBase - &vertexVector[0];
        if (graph.IsMatched(nextVertexBase_VertexID))
            TailOfAugmentingPath = &vertexVector[Graph<IT,VT>::Other(graph,graph.GetMatchField(nextVertexBase_VertexID),nextVertexBase_VertexID)];
        else 
            TailOfAugmentingPath = nullptr;
    } while (TailOfAugmentingPath != nullptr);
    // Print the list of integers
    for (auto E : path) {
        //Match(EdgeFrom(E)) = E;
        graph.SetMatchField(Graph<IT,VT>::EdgeFrom(graph,E),E);
        //Match(EdgeTo(E)) = E;
        graph.SetMatchField(Graph<IT,VT>::EdgeTo(graph,E),E);
    }
}


template <typename IT, typename VT>
void Matcher::extract_path(Graph<IT, VT>& graph, 
                    Vertex<IT> * TailOfAugmentingPath,
                    std::vector<Vertex<IT>> & vertexVector,
                    std::vector<IT> & path) {

    IT edge;
    // W
    Vertex<IT>*nextVertex;
    Vertex<IT>*nextVertexBase;
    do
    {
        //ListPut(Tree(V), P);
        edge = TailOfAugmentingPath->TreeField;
        path.push_back(edge);

        //W = Other(Tree(V), V);
        ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
        auto nextVertexID = Graph<IT,VT>::Other(graph,edge,TailOfAugmentingPath_VertexID);
        nextVertex = &vertexVector[nextVertexID];

        //B = Base(Blossom(W));
        // GJS
        auto nextVertexBaseID = DisjointSetUnionHelper<IT>::getBase(nextVertexID,vertexVector);  

        nextVertexBase = &vertexVector[nextVertexBaseID];
        
        // Path(W, B, P);
        pathThroughBlossomConcurrent(graph,nextVertex,nextVertexBase,vertexVector,path);

        //V = Other(Match(B), B);
        ptrdiff_t nextVertexBase_VertexID = nextVertexBase - &vertexVector[0];
        if (vertexVector[nextVertexBase_VertexID].MatchField>-1)
            TailOfAugmentingPath = &vertexVector[Graph<IT,VT>::Other(graph,vertexVector[nextVertexBase_VertexID].MatchField,nextVertexBase_VertexID)];
        else 
            TailOfAugmentingPath = nullptr;
    } while (TailOfAugmentingPath != nullptr);
}

template <typename IT, typename VT>
void Matcher::pathThroughBlossom(Graph<IT, VT>& graph, 
                    // V
                    const Vertex<IT> * TailOfAugmentingPath,
                    const Vertex<IT> * TailOfAugmentingPathBase,
                    std::vector<Vertex<IT>> & vertexVector,
                    //std::list<IT> & path,
                    std::vector<IT> & path) {
    // W
    Vertex<IT>*nextVertex;
    // if (V != B)
    if (TailOfAugmentingPath != TailOfAugmentingPathBase)
    {
        if (TailOfAugmentingPath->IsOdd())
        {
            // Path(Shore(V), Other(Match(V), V), P);
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            pathThroughBlossom(graph,
                                &vertexVector[TailOfAugmentingPath->ShoreField],
                                &vertexVector[Graph<IT,VT>::Other(graph,graph.GetMatchField(TailOfAugmentingPath_VertexID),TailOfAugmentingPath_VertexID)],
                                vertexVector,
                                path);
            //ListPut(Bridge(V), P);
            path.push_back(TailOfAugmentingPath->BridgeField);
            
            //Path(Other(Bridge(V), Shore(V)), B, P);
            pathThroughBlossom(graph,
                                &vertexVector[Graph<IT,VT>::Other(graph,TailOfAugmentingPath->BridgeField,TailOfAugmentingPath->ShoreField)],
                                TailOfAugmentingPathBase,
                                vertexVector,
                                path);
        }
        else if (TailOfAugmentingPath->IsEven())
        {
            //W = Other(Match(V), V);
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            nextVertex=&vertexVector[Graph<IT,VT>::Other(graph,graph.GetMatchField(TailOfAugmentingPath_VertexID),TailOfAugmentingPath_VertexID)];
            
            //ListPut(Tree(W), P);
            path.push_back(nextVertex->TreeField);

            //Path(Other(Tree(W), W), B, P);
            ptrdiff_t nextVertex_VertexID = nextVertex - &vertexVector[0];
            pathThroughBlossom(graph,
                                &vertexVector[Graph<IT,VT>::Other(graph,nextVertex->TreeField,nextVertex_VertexID)],
                                TailOfAugmentingPathBase,
                                vertexVector,
                                path);
        }
        else{
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            ptrdiff_t TailOfAugmentingPathBase_VertexID = TailOfAugmentingPathBase - &vertexVector[0];
            std::cerr << "(Path) Internal error. TailOfAugmentingPath_VertexID: " << TailOfAugmentingPath_VertexID<< " TailOfAugmentingPathBase_VertexID: " << TailOfAugmentingPathBase_VertexID << std::endl;
            exit(1);
        }
    }
}


template <typename IT, typename VT>
void Matcher::pathThroughBlossomConcurrent(Graph<IT, VT>& graph, 
                    // V
                    const Vertex<IT> * TailOfAugmentingPath,
                    const Vertex<IT> * TailOfAugmentingPathBase,
                    std::vector<Vertex<IT>> & vertexVector,
                    //std::list<IT> & path,
                    std::vector<IT> & path) {
    // W
    Vertex<IT>*nextVertex;
    // if (V != B)
    if (TailOfAugmentingPath != TailOfAugmentingPathBase)
    {
        if (TailOfAugmentingPath->IsOdd())
        {
            // Path(Shore(V), Other(Match(V), V), P);
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            pathThroughBlossomConcurrent(graph,
                                &vertexVector[TailOfAugmentingPath->ShoreField],
                                //&vertexVector[Graph<IT,VT>::Other(graph,graph.GetMatchField(TailOfAugmentingPath_VertexID),TailOfAugmentingPath_VertexID)],
                                &vertexVector[Graph<IT,VT>::Other(graph,vertexVector[TailOfAugmentingPath_VertexID].MatchField,TailOfAugmentingPath_VertexID)],
                                vertexVector,
                                path);
            //ListPut(Bridge(V), P);
            path.push_back(TailOfAugmentingPath->BridgeField);
            
            //Path(Other(Bridge(V), Shore(V)), B, P);
            pathThroughBlossomConcurrent(graph,
                                &vertexVector[Graph<IT,VT>::Other(graph,TailOfAugmentingPath->BridgeField,TailOfAugmentingPath->ShoreField)],
                                TailOfAugmentingPathBase,
                                vertexVector,
                                path);
        }
        else if (TailOfAugmentingPath->IsEven())
        {
            //W = Other(Match(V), V);
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            //nextVertex=&vertexVector[Graph<IT,VT>::Other(graph,graph.GetMatchField(TailOfAugmentingPath_VertexID),TailOfAugmentingPath_VertexID)];
            nextVertex=&vertexVector[Graph<IT,VT>::Other(graph,vertexVector[TailOfAugmentingPath_VertexID].MatchField,TailOfAugmentingPath_VertexID)];

            //ListPut(Tree(W), P);
            path.push_back(nextVertex->TreeField);

            //Path(Other(Tree(W), W), B, P);
            ptrdiff_t nextVertex_VertexID = nextVertex - &vertexVector[0];
            pathThroughBlossomConcurrent(graph,
                                &vertexVector[Graph<IT,VT>::Other(graph,nextVertex->TreeField,nextVertex_VertexID)],
                                TailOfAugmentingPathBase,
                                vertexVector,
                                path);
        }
        else{
            ptrdiff_t TailOfAugmentingPath_VertexID = TailOfAugmentingPath - &vertexVector[0];
            ptrdiff_t TailOfAugmentingPathBase_VertexID = TailOfAugmentingPathBase - &vertexVector[0];
            std::cerr << "(Path) Internal error. TailOfAugmentingPath_VertexID: " << TailOfAugmentingPath_VertexID<< " TailOfAugmentingPathBase_VertexID: " << TailOfAugmentingPathBase_VertexID << std::endl;
            exit(1);
        }
    }
}

#endif

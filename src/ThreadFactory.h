#ifndef THREAD_FACTORY2_H
#define THREAD_FACTORY2_H
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <cassert>
#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "Graph.h"
#include "Frontier.h"
#include "Matcher.h"

struct BenchResult {
  size_t num_readers;
  size_t written_messages;
  std::vector<size_t> read_messages;
  std::chrono::milliseconds duration;
};

void print_results(const BenchResult &results) {
  size_t tot_read_messages = 0;
  int ni = 0;
  for (size_t n : results.read_messages){
    printf("reader: \t\t %d read_messages %zu \n", ni++,n);
    tot_read_messages += n;
  }

  printf("duration: \t\t %zu millseconds\n", results.duration.count());
  printf("num_readers: \t\t %zu reader\n", results.num_readers);
  printf("written_msgs: \t\t %f message/millseconds\n",
         (float)results.written_messages / (results.duration.count()));
  printf("avg_read_msgs: \t\t %f message/millseconds\n",
         (float)(tot_read_messages / results.num_readers) /
             results.duration.count());
  printf("\n");
}


#include <iostream>
#include <vector>
#include <thread>
#include "concurrentqueue.h"

class ThreadFactory {
public:

    //template <typename IT, typename VT>
    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
    static bool create_threads_concurrentqueue_wl(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    std::atomic<bool>& found_augmenting_path,
                                                    std::vector<std::mutex> &worklistMutexes,
                                                    std::vector<std::condition_variable> &worklistCVs,
                                                    std::atomic<IT> & num_enqueued,
                                                    std::atomic<IT> & num_dequeued,
                                                    std::atomic<IT> & num_contracting_blossoms,
                                                    int deferral_threshold);


    template <typename IT, typename VT>
    static bool create_threads_concurrentqueue_wl2(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    moodycamel::ConcurrentQueue<IT> &worklist,
                                                    std::vector<std::atomic_flag> &dead,
                                                    std::atomic<bool> &finished,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    int deferral_threshold);


    template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType=std::vector>
    static bool create_threads_concurrentqueue_wl3(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    std::atomic<bool>& found_augmenting_path,
                                                    std::vector<std::mutex> &worklistMutexes,
                                                    std::vector<std::condition_variable> &worklistCVs,
                                                    std::atomic<IT> & num_enqueued,
                                                    std::atomic<IT> & num_dequeued,
                                                    std::atomic<IT> & num_contracting_blossoms,
                                                    int deferral_threshold);

};



//template <typename IT, typename VT>
template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
bool ThreadFactory::create_threads_concurrentqueue_wl(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    std::atomic<bool>& found_augmenting_path,
                                                    std::vector<std::mutex> &worklistMutexes,
                                                    std::vector<std::condition_variable> &worklistCVs,
                                                    std::atomic<IT> & num_enqueued,
                                                    std::atomic<IT> & num_dequeued,
                                                    std::atomic<IT> & num_contracting_blossoms,
                                                    int deferral_threshold) {
    // Works, infers template types from args
    //Matcher::search(graph,0,*(frontiers[0]));
    for (unsigned i = 0; i < num_threads; ++i) {
        //threads[i] = std::thread(&Matcher::hello_world, i);
        if (deferral_threshold) {
          threads[i] = std::thread( [&,i,deferral_threshold]{ Matcher::match_persistent_wl5<IT,VT>(graph,
            worklists,masterTID,
            read_messages,found_augmenting_path,
            currentRoot,
            worklistMutexes,worklistCVs,i,num_enqueued,num_dequeued,num_contracting_blossoms,
            deferral_threshold); } );
        } else {
          threads[i] = std::thread( [&,i]{ Matcher::match_persistent_wl4<IT,VT>(graph,
            masterTID,
            read_messages,found_augmenting_path,
            currentRoot,
            worklistMutexes,worklistCVs,i); } );
        }
        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
        // only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                        sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
  return true;
}


//template <typename IT, typename VT>
template <typename IT, typename VT>
bool ThreadFactory::create_threads_concurrentqueue_wl2(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    moodycamel::ConcurrentQueue<IT> &worklist,
                                                    std::vector<std::atomic_flag> &dead,
                                                    std::atomic<bool> &finished,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    int deferral_threshold){
    // Works, infers template types from args
    //Matcher::search(graph,0,*(frontiers[0]));
    for (unsigned i = 0; i < num_threads; ++i) {
        //threads[i] = std::thread(&Matcher::hello_world, i);
          threads[i] = std::thread( [&,i,deferral_threshold]{ Matcher::match_persistent_wl7<IT,VT>(graph,
            worklist,dead,finished,masterTID,
            read_messages,
            currentRoot,
            i,
            deferral_threshold); } );
        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
        // only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                        sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
  return true;
}


//template <typename IT, typename VT>
template <typename IT, typename VT, template <typename, template <typename> class> class FrontierType, template <typename> class StackType>
bool ThreadFactory::create_threads_concurrentqueue_wl3(std::vector<std::thread> &threads,
                                                    unsigned num_threads,
                                                    std::vector<size_t> &read_messages,
                                                    std::vector<moodycamel::ConcurrentQueue<FrontierType<IT, StackType>, moodycamel::ConcurrentQueueDefaultTraits>> &worklists,
                                                    std::atomic<IT> &masterTID,
                                                    Graph<IT, VT> &graph,
                                                    std::atomic<IT> & currentRoot,
                                                    std::atomic<bool>& found_augmenting_path,
                                                    std::vector<std::mutex> &worklistMutexes,
                                                    std::vector<std::condition_variable> &worklistCVs,
                                                    std::atomic<IT> & num_enqueued,
                                                    std::atomic<IT> & num_dequeued,
                                                    std::atomic<IT> & num_contracting_blossoms,
                                                    int deferral_threshold) {
    // Works, infers template types from args
    //Matcher::search(graph,0,*(frontiers[0]));
    for (unsigned i = 0; i < num_threads; ++i) {
        //threads[i] = std::thread(&Matcher::hello_world, i);

        threads[i] = std::thread( [&,i]{ Matcher::match_persistent_wl4<IT,VT>(graph,
          masterTID,
          read_messages,found_augmenting_path,
          currentRoot,
          worklistMutexes,worklistCVs,i); } );
        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
        // only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                        sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
  return true;
}

#endif
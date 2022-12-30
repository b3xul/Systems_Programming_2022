// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <fstream>

#include <memory>
#include <unistd.h>
#include <sys/wait.h>

#include <vector>
#include <set>
#include <map>

#include "MapperInput.hpp"
#include "ReducerInput.hpp"
#include "Result.hpp"

#include "../../../Personal_Study/Timing_Visual_Profiler/instrumentor.h"

//#include <boost/json/src.hpp>

#define MAPPER    "[ MAPPER ]\t"
#define REDUCER   "[ REDUCER ]\t"
#define COORDINATOR    "[ COORDINATOR ]\t"

template<typename MapperInputT>
std::string extract_ip(const MapperInputT &input) {
  std::size_t last_char_ip =
      input.get_value().find(' '); //position of ' ' is also the length of the substring to be __extracted
  std::string ip = input.get_value().substr(0, last_char_ip);
  //std::cout << ip << '\n';
  return ip;
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_ip(const MapperInputT &input) {

  return std::vector<ResultT>{Result(extract_ip(input), 1)};
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_hour(const MapperInputT &input) {
  std::size_t first_char_hour = input.get_value().find(':') + 1; // starts from after the :
  std::size_t last_char_hour = input.get_value().find(':', first_char_hour);
  std::size_t substring_length = last_char_hour - first_char_hour;
  std::string hour = input.get_value().substr(first_char_hour, substring_length);
  //std::cout << hour << '\n';
  return std::vector<ResultT>{Result(hour, 1)};
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_url(const MapperInputT &input) {
  std::size_t first_char_url = input.get_value().find(" /") + 1; // starts from /
  std::size_t last_char_url = input.get_value().find(' ', first_char_url);
  std::size_t substring_length = last_char_url - first_char_url;
  std::string url = input.get_value().substr(first_char_url, substring_length);
  //std::cout << url << '\n';
  return std::vector<ResultT>{Result(url, 1)};
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_code(const MapperInputT &input) {
  std::size_t first_char_code = input.get_value().find("\" ") + 2; // starts from the first number of the code
  std::size_t last_char_code = input.get_value().find(' ', first_char_code);
  std::size_t substring_length = last_char_code - first_char_code;
  std::string code = input.get_value().substr(first_char_code, substring_length);
  //std::cout << code << '\n';
  std::string ip = extract_ip(input);
  std::set<std::string> ips{ip};
  return std::vector<ResultT>{Result(code, ips)};
}

template<typename ReducerInputT, typename ResultT>
ResultT reduceAcc(const ReducerInputT &input) {
  return Result(input.get_key(), input.get_val() + input.get_accumulator());
}

template<typename ReducerInputT, typename ResultT>
ResultT reduceString(const ReducerInputT &input) {
  // input.get_val() array of 1 string
  // input.get_accumulator() array of many strings
  // ESSENTIAL TO STORE THEM INTO VARIABLES INSTEAD OF ACCESSING THEM MULTIPLE TIMES!
  std::set<std::string> val = input.get_val();
  std::set<std::string> acc = input.get_accumulator();
  std::string key = input.get_key();

  //for (auto i = val.begin(); i != val.end(); i++) std::cout << *i << std::endl;

  std::set<std::string> res;
  //res.reserve(acc.size() + val.size());
  res.insert(acc.begin(), acc.end());
  res.insert(val.begin(), val.end());
  return Result(key, res);
}

template<class T>
void write_to_pipe(int fd, const T &obj) {
  std::vector<char> v = obj.serialize(); // will call the right version of the serialize function
  /*  for (const char &c: v) {
      std::cout << c;
    }*/
  //std::cout << v.data();
  //std::cout << v.size();
  std::size_t dim = v.size();
  std::size_t bytes_written = write(fd, &dim, sizeof(std::size_t));
  bytes_written += write(fd, v.data(), v.size());
  //std::cout << "... done (someone written " << bytes_written << " bytes)" << std::endl;
}
template<class T>
T read_from_pipe(int fd) {
  std::string message_from_pipe;
  std::size_t l;
  std::size_t bytes_read;
  T object;
  if ((bytes_read = read(fd, &l, sizeof(std::size_t))) > 0) {
    //std::cout << MAPPER << "Bytes received: " << bytes_read << "-> l=" << l << '\n';
    message_from_pipe.resize(l);
    bytes_read += read(fd, &message_from_pipe[0], message_from_pipe.size());
    //std::cout << MAPPER << "Bytes received: " << bytes_read << "-> Message from the COORDINATOR: " << message_from_pipe<< '\n';

    object.deserialize(std::make_shared<char *>(message_from_pipe.data()));
  }
  return object;
}

// KeyT and ValueT are the same used in ResultT
template<typename KeyT, typename ValueT, typename MapperInputT, typename ReducerInputT, typename ResultT>
std::map<KeyT, ValueT> mapreduce(const std::string &input_filename,
                                 std::vector<ResultT> mapF(const MapperInputT &input),
                                 ResultT reduceF(const ReducerInputT &input)) {
  //std::cout << "COORDINATOR STARTS EXECUTION\n";
  std::string line;
  std::map<KeyT, ValueT> results;
  std::ifstream input_file(input_filename);

  pid_t pid_after_fork;
  pid_t pid_after_fork_2;

  int status;
  int coordinatorToMapper[2];
  int mapperToCoordinator[2];
  int coordinatorToReducer[2];
  int reducerToCoordinator[2];
  int pipe_error;

  if (input_file.is_open()) {
    if ((pipe_error = pipe(coordinatorToMapper)) != 0) {
      std::cerr << "Error on opening pipe\n";
      exit(pipe_error);
    }
    if ((pipe_error = pipe(mapperToCoordinator)) != 0) {
      std::cerr << "Error on opening pipe\n";
      exit(pipe_error);
    }
    if ((pipe_error = pipe(coordinatorToReducer)) != 0) {
      std::cerr << "Error on opening pipe\n";
      exit(pipe_error);
    }
    if ((pipe_error = pipe(reducerToCoordinator)) != 0) {
      std::cerr << "Error on opening pipe\n";
      exit(pipe_error);
    }

    pid_after_fork = fork();

    switch (pid_after_fork) {
      case 0: {
        close(coordinatorToMapper[1]);
        close(mapperToCoordinator[0]);
        //std::cout << MAPPER << "My PID: " << getpid() << '\n';

        bool still_reading = true;
        while (still_reading) {
          MapperInputT mapper_input = read_from_pipe<MapperInputT>(coordinatorToMapper[0]);
          if (mapper_input.get_value() != "") {
            std::vector<ResultT> mapped;
            mapped = mapF(mapper_input);
            for (const ResultT &i: mapped)
              write_to_pipe(mapperToCoordinator[1], i);
            write_to_pipe(mapperToCoordinator[1], ResultT(""));
          } else
            still_reading = false;
        }
        exit(1);
      }
      case -1: {
        //std::cout << "mapper fork error" << std::endl;
        exit(-1);
      }
        break;
      default: {
        close(coordinatorToMapper[0]);
        close(mapperToCoordinator[1]);
        //std::cout << COORDINATOR << "My PID: " << getpid() << '\n' << COORDINATOR << "\t-> Mapper: " <<
        // pid_after_fork << '\n';
        pid_after_fork_2 = fork();
        switch (pid_after_fork_2) {
          case 0: {
            close(coordinatorToReducer[1]);
            close(reducerToCoordinator[0]);
            //std::cout << REDUCER << "My PID: " << getpid() << '\n';
            bool still_reading = true;
            while (still_reading) {
              ReducerInputT reducer_input = read_from_pipe<ReducerInputT>(coordinatorToReducer[0]);
              if (reducer_input.get_key() != "") {
                ResultT new_result = reduceF(ReducerInput(reducer_input.get_key(),
                                                          reducer_input.get_val(),
                                                          reducer_input.get_accumulator()));

                write_to_pipe(reducerToCoordinator[1], new_result);
                write_to_pipe(reducerToCoordinator[1], ResultT(""));
              } else
                still_reading = false;
            }
            exit(2);
          }
          case -1: {
            //std::cout << "reducer fork error" << std::endl;
            exit(EXIT_FAILURE);
          }
            break;
          default: {
            close(coordinatorToReducer[0]);
            close(reducerToCoordinator[1]);
            //std::cout << COORDINATOR << "My PID: " << getpid() << '\n' << COORDINATOR << "\t-> Reducer: "<< pid_after_fork_2 << '\n';
            while (std::getline(input_file, line)) {
              //printf("%s\n", line.c_str());
              write_to_pipe(coordinatorToMapper[1], MapperInputT(line));
              std::vector<ResultT> map_results;
              bool still_reading = true;
              while (still_reading) {
                ResultT r = read_from_pipe<ResultT>(mapperToCoordinator[0]);
                if (r.get_key() != "") {
                  map_results.push_back(r);
                } else
                  still_reading = false;
              }

              for (const ResultT &r: map_results) {
                write_to_pipe(coordinatorToReducer[1], ReducerInput(r.get_key(), r.get_value(), results[r.get_key()]));
                still_reading = true;
                while (still_reading) {
                  ResultT new_result = read_from_pipe<ResultT>(reducerToCoordinator[0]);
                  if (new_result.get_key() != "") {
                    results[r.get_key()] = new_result.get_value();
                  } else
                    still_reading = false;
                }
              }
            }
            write_to_pipe(coordinatorToMapper[1], MapperInputT(""));
            write_to_pipe(coordinatorToReducer[1], ReducerInputT(""));
            wait(&status);
            //std::cout << COORDINATOR << "Child Status: " << WEXITSTATUS(status) << " (" << status << ")\n";
            wait(&status);
            //std::cout << COORDINATOR << "Child Status: " << WEXITSTATUS(status) << " (" << status << ")\n";
          }
        }
      }
    }
    input_file.close();
  }
  return results;
}

// 4 pipes:
// coordinatore legge "209.17.96.34 - - [01/Jan/2020:00:19:59 +0100] "GET / HTTP/1.1" 200 20744"=MapperInputT e lo
// manda al mapper
// mapper mappa e risponde al coordinatore [ ("209.17.96.34", 1) ] (in questo caso 1 solo elemento nel vettore )
// (piu' mapper potrebbero lavorare in parallelo)
// coordinatore manda risultati man mano al reducer [ ("209.17.96.34", 1, 0) ] o [ ("209.17.96.34", 1,1 )]=ReducerInputT
// reducer riduce e risponde al coordinatore ("209.17.96.34", 2) o ("10.0.3.2",4)

void Timing() {
  PROFILE_FUNCTION();
  typedef MapperInput<std::string> MapperInputBrief;
  typedef ReducerInput<std::string, int, int> ReducerInputBrief;
  typedef ReducerInput<std::string, std::set<std::string>, std::set<std::string>> ReducerInputStringBrief;
  typedef Result<std::string, int> ResultBrief;
  typedef Result<std::string, std::set<std::string>> ResultStringBrief;

  const std::string input_filename = "./../log.txt";

  std::map<std::string, int> requests_by_ip =
      mapreduce<std::string, int, MapperInputBrief, ReducerInputBrief, ResultBrief>(input_filename,
                                                                                    map_ip<MapperInputBrief,
                                                                                           ResultBrief>,
                                                                                    reduceAcc<ReducerInputBrief,
                                                                                              ResultBrief>);

  for (const auto&[key, value]: requests_by_ip)
    std::cout << '(' << key << ", " << value << ")\n";

  std::map<std::string, int> requests_by_hour =
      mapreduce<std::string, int, MapperInputBrief, ReducerInputBrief, ResultBrief>(input_filename,
                                                                                    map_hour<MapperInputBrief,
                                                                                             ResultBrief>,
                                                                                    reduceAcc<ReducerInputBrief,
                                                                                              ResultBrief>);

  for (const auto&[key, value]: requests_by_hour)
    std::cout << '(' << key << ", " << value << ")\n";

  std::map<std::string, int> requests_by_url =
      mapreduce<std::string, int, MapperInputBrief, ReducerInputBrief, ResultBrief>(input_filename,
                                                                                    map_url<MapperInputBrief,
                                                                                            ResultBrief>,
                                                                                    reduceAcc<ReducerInputBrief,
                                                                                              ResultBrief>);

  for (const auto&[key, value]: requests_by_url)
    std::cout << '(' << key << ", " << value << ")\n";

  std::map<std::string, std::set<std::string>> ip_by_code =
      mapreduce<std::string, std::set<std::string>, MapperInputBrief, ReducerInputStringBrief, ResultStringBrief>(
          input_filename,
          map_code<MapperInputBrief, ResultStringBrief>,
          reduceString<ReducerInputStringBrief, ResultStringBrief>);

  for (const auto&[key, value]: ip_by_code) {
    if (key[0] == '4') {
      std::cout << '(' << key << ", [ ";
      for (const auto &v: value)
        std::cout << v << ' ';
      std::cout << "])\n";
    }
  }

}
int main() {
  Instrumentor::Instance().beginSession("Profile1");
  Timing(); //19.988, 55.210, 120
  Instrumentor::Instance().endSession();

  return 0;
}
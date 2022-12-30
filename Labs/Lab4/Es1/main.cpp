#include <iostream>
#include <fstream>

#include <vector>
#include <set>
#include <map>
#include "MapperInput.hpp"
#include "ReducerInput.hpp"
#include "Result.hpp"

#include "../../../Personal_Study/Timing_Visual_Profiler/instrumentor.h"

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

  return std::vector < ResultT > {Result(extract_ip(input), 1)};
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_hour(const MapperInputT &input) {
  std::size_t first_char_hour = input.get_value().find(':') + 1; // starts from after the :
  std::size_t last_char_hour = input.get_value().find(':', first_char_hour);
  std::size_t substring_length = last_char_hour - first_char_hour;
  std::string hour = input.get_value().substr(first_char_hour, substring_length);
  std::cout << hour << '\n';
  return std::vector < ResultT > {Result(hour, 1)};
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_url(const MapperInputT &input) {
  std::size_t first_char_url = input.get_value().find(" /") + 1; // starts from /
  std::size_t last_char_url = input.get_value().find(' ', first_char_url);
  std::size_t substring_length = last_char_url - first_char_url;
  std::string url = input.get_value().substr(first_char_url, substring_length);
  std::cout << url << '\n';
  return std::vector < ResultT > {Result(url, 1)};
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
  return std::vector < ResultT > {Result(code, ips)};
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

// KeyT and ValueT are the same used in ResultT
template<typename KeyT, typename ValueT, typename MapperInputT, typename ReducerInputT, typename ResultT>
std::map<KeyT, ValueT> mapreduce(const std::string &input_filename,
                                 std::vector<ResultT> mapF(const MapperInputT &input),
                                 ResultT reduceF(const ReducerInputT &input)) {
  std::cout << "COORDINATOR STARTS EXECUTION\n";
  std::string line;
  std::vector<ResultT> mapped;
  std::map<KeyT, ValueT> results;
  ResultT new_result;
  std::ifstream input_file(input_filename);

  if (input_file.is_open()) {
    while (std::getline(input_file, line)) {
      printf("%s\n", line.c_str());
      mapped = mapF(MapperInputT(line));
      for (const ResultT &r: mapped) {
        new_result = reduceF(ReducerInput(r.get_key(), r.get_value(), results[r.get_key()]));
        results[r.get_key()] = new_result.get_value();
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
  typedef ReducerInput<std::string, std::set < std::string>, std::set < std::string >> ReducerInputStringBrief;
  typedef Result<std::string, int> ResultBrief;
  typedef Result<std::string, std::set < std::string>>
  ResultStringBrief;

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

  std::map < std::string, std::set < std::string >> ip_by_code =
                              mapreduce<std::string, std::set < std::string>, MapperInputBrief, ReducerInputStringBrief,
      ResultStringBrief > (input_filename, map_code<MapperInputBrief, ResultStringBrief>, reduceString<
          ReducerInputStringBrief,
          ResultStringBrief>);

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
  Timing(); //21,1,1,13
  Instrumentor::Instance().endSession();

  return 0;
}
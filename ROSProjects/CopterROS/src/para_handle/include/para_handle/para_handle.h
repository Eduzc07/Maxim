#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <vector>
#include <atomic>
#include <string>
#include <thread>
#include <mutex>
#include "ros/ros.h"

class ParaHandle
{
private:
  static ParaHandle* _instance;

  std::vector<std::string> _int_param_names;
  std::vector<std::string> _float_param_names;
  std::vector<std::atomic<int>*> _int_param_value_ptrs;
  std::vector<std::atomic<float>*> _float_param_value_ptrs;

  std::mutex _new_int_param_mutex;
  std::mutex _new_float_param_mutex;

  float _refresh_rate;

  std::thread _param_refresher;
  void refresh_thread();

  ParaHandle() { }
  ParaHandle( const ParaHandle& );

  void check_all_int_Parameter();
  void check_all_float_Parameter();
  void check_all_string_Parameter();



public:
  void init(float refresh_rate);
  static ParaHandle* get(){
    if(! _instance)
       _instance = new ParaHandle();
    return _instance;
  }
  ~ParaHandle();
  bool stringParamOnce(std::string name,std::string* value_ptr);
  bool floatParam(std::string name,std::atomic<float>* value_ptr);
  bool intParam(std::string name,std::atomic<int>* value_ptr);
  bool floatParamOnce(std::string name,float* value_ptr);
  bool intParamOnce(std::string name,int* value_ptr);





};


#endif // __PARAMETER_H__

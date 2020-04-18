#include "para_handle/para_handle.h"

ParaHandle* ParaHandle::_instance = NULL;


void ParaHandle::init(float refresh_rate){
  _refresh_rate = refresh_rate;
  _param_refresher = std::thread(&ParaHandle::refresh_thread, this);
  return;
}

bool ParaHandle::stringParamOnce(std::string name,std::string* value_ptr){
  std::string tmp;
  if(!ros::param::get(name,tmp)){
    ROS_ERROR_STREAM("parameter "<< name << " does not exist, or is not string");
    ros::shutdown();
    return false; 
  }
  *value_ptr = tmp;
  return true;
}

bool ParaHandle::floatParam(std::string name,std::atomic<float>* value_ptr){
  std::unique_lock<std::mutex> lck(_new_float_param_mutex);
  float tmp;
  if(!ros::param::get(name,tmp)){
    ROS_ERROR_STREAM("parameter "<< name << " does not exist, or is not float");
    ros::shutdown();
    return false; 
  }
  *value_ptr = tmp;
  _float_param_names.push_back(name);
  _float_param_value_ptrs.push_back(value_ptr);
  return true;
}

bool ParaHandle::floatParamOnce(std::string name,float* value_ptr){
  float tmp;
  if(!ros::param::get(name,tmp)){
    ROS_ERROR_STREAM("parameter "<< name << " does not exist, or is not float");
    ros::shutdown();
    return false; 
  }
  *value_ptr = tmp;
  return true;
}

bool ParaHandle::intParam(std::string name,std::atomic<int>* value_ptr){
  std::unique_lock<std::mutex> lck(_new_int_param_mutex);
  int tmp;
  if(!ros::param::get(name,tmp)){
    ROS_ERROR_STREAM("parameter "<< name << " does not exist, or is not integer");
    ros::shutdown();
    return false; 
  }
  *value_ptr = tmp;
  _int_param_names.push_back(name);
  _int_param_value_ptrs.push_back(value_ptr);
  return true;
}

bool ParaHandle::intParamOnce(std::string name,int* value_ptr){
  int tmp;
  if(!ros::param::get(name,tmp)){
    ROS_ERROR_STREAM("parameter "<< name << " does not exist, or is not integer");
    ros::shutdown();
    return false; 
  }
  *value_ptr = tmp;
  return true;
}

void ParaHandle::refresh_thread(){
  ros::Rate rate(5.0);
  while (ros::ok()){
    check_all_int_Parameter();
    check_all_float_Parameter();


    rate.sleep();
  }


}

void ParaHandle::check_all_int_Parameter(){
  if (_int_param_names.size()!=_int_param_value_ptrs.size())
    ROS_ERROR_STREAM("Parameter Error (int_param)");
  std::unique_lock<std::mutex> lck(_new_int_param_mutex);
  for (unsigned int i = 0; i < _int_param_names.size(); i++){
    int tmp;
    if(!ros::param::get(_int_param_names[i],tmp)){
      ROS_ERROR_STREAM("parameter "<< _int_param_names[i] << " is no longer available -_-");
    }
    if (tmp != *_int_param_value_ptrs[i])
      *_int_param_value_ptrs[i] = tmp;  
  }
}

void ParaHandle::check_all_float_Parameter(){
  if (_float_param_names.size()!=_float_param_value_ptrs.size())
    ROS_ERROR_STREAM("Parameter Error (float_param)");
  std::unique_lock<std::mutex> lck(_new_float_param_mutex);
  for (unsigned int i = 0; i < _float_param_names.size(); i++){
    float tmp;
    if(!ros::param::get(_float_param_names[i],tmp)){
      ROS_ERROR_STREAM("parameter "<< _float_param_names[i] << " is no longer available -_-");
    }
    if (tmp != *_float_param_value_ptrs[i])
      *_float_param_value_ptrs[i] = tmp;  
  }
}















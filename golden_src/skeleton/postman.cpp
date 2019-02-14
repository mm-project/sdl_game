#include "postman.h"
#include "callback.h"

#include <iostream>
#include <cassert>
#include <string>

//LePostman::LePostman() {
//    m_types_count = 0;
//}

LeCallbackType LePostman::add_callback_type(const std::string& name) {
    //LeCallbackType cbt(name);
    LeCallbackType cbt;    
    m_index2type[++m_types_count] = cbt;
    return cbt;
}

//LeCallbackType LePostman::get_callbacktype(const std::string&) {
//        
//}


LeCallback LePostman::register_callback(const LeCallbackType& t,callBackFun1 f) {
    m_type2vecfun[t].push_back(LeCallback(f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

LeCallback LePostman::register_callback(const LeCallbackType& t,callBackFun2 f) {
    m_type2vecfun[t].push_back(LeCallback(f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

void LePostman::deregister_callback(LeCallbackType& t, int id) {
    m_type2vecfun[t].erase(m_type2vecfun[t].begin()+id); 
}

void LePostman::notify(LeCallbackType& t) {
    std::cout << "notify" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        it.call();
    }
}

void LePostman::notify(LeCallbackType& t,LeCallbackData& data) {
    std::cout << "notify" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        it.call(data);
    }
}

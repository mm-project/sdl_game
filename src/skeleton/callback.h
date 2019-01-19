#ifndef callback_h
#define callback_h

#include <functional>

#include <string>


class LeCallbackData 
{
    public:
        virtual ~LeCallbackData() {}
};


//enum LeCallbackType {CB_GAMEOVER};
typedef std::string LeCallbackType;

/*
class LeCallbackType
{
    public:
        LeCallbackType() {}
        LeCallbackType(const std::string& s):m_name(s) {}
        std::string get_name() { return m_name; }
        virtual ~LeCallbackType() {}
        bool operator < (LeCallbackType t) const {
        return true;
            
        }

    private:
        std::string m_name; 
};
*/

typedef std::function<void(LeCallbackData&)> callBackFun1;
typedef std::function<void()> callBackFun2;


class LeCallback 
{
    friend class LePostman;
    
    public:
        void purge();
        void call();
        void call(LeCallbackData& data);
        
    private:
        LeCallback(callBackFun1 f1, int id, const LeCallbackType& t);
        LeCallback(callBackFun2 f2, int id, const LeCallbackType& t);
        
    private:
        callBackFun1 m_f1;
        callBackFun2 m_f2;
        int m_internal_id;
        LeCallbackType m_type;
        bool m_is_purged;
};

#endif

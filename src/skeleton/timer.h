#ifndef timer_h
#define timer_h

#include "common.h"

#include <vector>


class LeTimer {
	
	public: 
		LeTimer ();
		void add_continuous_checkpoint(const updateFun& f, unsigned int t);
		void add_singleshot_checkpoint(const updateFun& f, unsigned int t);
		
		void step(unsigned int current_time );
	
	private:
		struct LeHavaqakanStruktura {
			LeHavaqakanStruktura(const updateFun& f = nullptr, unsigned t = 0, unsigned g = 0):f(f),freq(t),last_time(g) {}
			updateFun f;
			unsigned int last_time;
			unsigned int freq;
		};
		
	private:
		std::vector<LeHavaqakanStruktura> m_structure;  
		std::vector<LeHavaqakanStruktura> m_structure2;  
		uint m_fired;
		
};
#endif

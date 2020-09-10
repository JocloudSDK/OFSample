#pragma once
#ifndef __nsox_nutimer_h
#define __nsox_nutimer_h

#include <algorithm>
#include <vector>


namespace nsox{


struct timer_handler
{
		virtual void timeout() = 0;
		virtual void Release() = 0;
};

#define TVN_BITS (8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVN_MASK (TVN_SIZE - 1)

#define MAKE_TIMER_ID(slot, id)  ((int)(((BYTE)((DWORD_PTR)(id) & 0xffffff)) | ((int)((BYTE)((DWORD_PTR)(slot) & 0xff))) << 24))

#define TIMER_FD(id)	((int)((DWORD_PTR)(id) & 0xffffff))
#define TIMER_SLOT(id)    ((WORD)((DWORD_PTR)(id) >> 24))


typedef int timer_span;
typedef int timer_id;



class nutimer
{
protected:
	struct timer_item
	{
			timer_item() : handler(NULL), fd(0) { };
			timer_item(timer_handler* h):handler(h),fd(0){}

			timer_handler* handler;
			timer_id	   fd;
	};

	typedef std::vector<timer_item>		timer_list;
	typedef timer_list::iterator		timer_list_itr;				
	typedef timer_list::iterator		timer_pos;
	typedef std::map<int, timer_pos>	timer_pos_map;

	static bool timer_compare(const timer_item& a, const timer_item& b)
	{
		return a.fd < b.fd;
	}

	
public:
	nutimer(int tick_span = 50)
	{
			__timer_fd		= 1;
			__tick_span		= tick_span;
			__elapse_tick	= 0;
			__ticking		= false;// 正在ticking
	}
	~nutimer(void){
	
		//释放所有的定时器
		for(int i=0; i< sizeof(__timers)/sizeof(__timers[0]); ++ i)
		{
			timer_list& list = __timers[i];
			for(timer_list_itr itr = list.begin(); itr != list.end(); ++ itr)
			{
				if(itr->handler) itr->handler->Release();
			}
			list.clear(); 
		}
	}

public:

	timer_id add_timeout(int timespan, timer_handler* handler)
	{
			return __add_timeout(timespan, handler);
	}
	void	del_timeout(timer_id id)
	{
			return __del_timeout(id);
	}
	int		tickaway(int elapse)
	{						
			__tickaway(elapse/__tick_span);
			return elapse % __tick_span;
	}

	void	notify_all()
	{
				__tickaway(TVN_SIZE);
	}

	
protected:
	timer_list_itr locate(timer_list& tlist, int fd)
	{
			__compItem.fd = fd;
			return std::lower_bound(tlist.begin(), tlist.end(), __compItem, timer_compare);
	}


	timer_id __add_timeout(int timespan, timer_handler* handler) // 添加定时器
	{		
		if(timespan < 0)
		{
			DebugBreak();
		}

		int slots = timespan/__tick_span ;
		if(!slots)  {
				slots ++;
		}
		timer_item item(handler);
		if(slots < TVN_SIZE )
		{
				int slot = (slots + __elapse_tick) & TVN_MASK;
				item.fd  = __timer_fd ++;

				timer_list& tlist = __timers[slot];
				timer_list_itr itr = locate(tlist, item.fd);
				tlist.insert(itr, item);

				return MAKE_TIMER_ID(slot, item.fd);
		}else{
				return 0;
		}
	}
	void __del_timeout(timer_id id)
	{
			int slot  = TIMER_SLOT(id);
			int fd	  = TIMER_FD(id);

			timer_list& tlist = __timers[slot];
			timer_list_itr itr = locate(tlist, fd);
			if(itr != tlist.end() && itr->fd == fd )
			{
				itr->handler = NULL;
			}
	}

	void	__tickaway(int ticks)
	{
			__ticking = true;

			for(int i=0; i<ticks; ++ i)
			{
				__elapse_tick = (++__elapse_tick) & TVN_MASK;
					
				timer_list& list = __timers[__elapse_tick];
				for(timer_list_itr itr = list.begin(); itr != list.end(); ++ itr)
				{
#ifndef	_DEBUG
					try
					{
#endif
						if(itr->handler) itr->handler->timeout();

#ifndef _DEBUG
					}
					catch (...)
					{
						
					}
#endif 
				}
				list.clear(); 
			}

			__ticking = false;
	}
protected:
	timer_list				__timers[TVN_SIZE];

	int						__tick_span;
	int						__elapse_tick;
	int						__timer_fd;		// maintain ids, BUGBUG, how to recycle id
	boolean					__ticking;
	timer_item				__compItem;
};

}
#endif

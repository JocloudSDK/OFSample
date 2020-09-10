#pragma once

namespace{
char *name = 0;
bool enableLog = false;
int heapMark = 0;
int tickCount = 0;
quint32 totalMemory = 0;
Qt::HANDLE mainThreadId = 0;
static bool enableAllocHook = true;
int YYAllocHook(int nAllocType, void *pvData,
				size_t nSize, int nBlockUse, long lRequest,
				const unsigned char * szFileName, int nLine )
{
	nLine;
	szFileName;
	lRequest;

	if ( nBlockUse == _CRT_BLOCK )
	{
		return( TRUE );
	}

	if(!enableAllocHook)
	{
		return TRUE;
	}

	enableAllocHook = false;

	if(mainThreadId == QThread::currentThreadId())
	{
		switch(nAllocType)
		{
		case _HOOK_ALLOC:
			totalMemory += nSize;
			tickCount++;
			if(enableLog)
			{
				qDebug() << "[HEAP MARK] Alloc: " << nSize;
			}
			break;
		case _HOOK_REALLOC:
			//			totalMemory += nSize;
			//			tickCount++;
			//qDebug() << "Hook ReAlloc: " << nSize;
			break;

		case _HOOK_FREE:
			totalMemory -= _msize(pvData);
			if(enableLog)
			{
				qDebug() << "[HEAP MARK] Free: " << nSize;
			}
			break;

		default:
			break;
		}

		if(tickCount % 1000 == 0)
		{
			qDebug() << "Total alloc memory: " << totalMemory;
		}
	}

	enableAllocHook = true;
	return TRUE;
}
}
void begin_heap_mark()
{
	mainThreadId = QThread::currentThreadId();
	_CrtSetAllocHook(YYAllocHook);
	totalMemory = 0;
}

extern int end_heap_mark()
{
	_CrtSetAllocHook(NULL);
	return totalMemory;	
}

#define BEGIN_HEAPMARK(x) begin_heap_mark(); name = #x;
#define END_HEAPMARK() qDebug() << "[HEAP MARK]: " << QString("%1").arg(end_heap_mark(), 8) << "\t\t" << name;

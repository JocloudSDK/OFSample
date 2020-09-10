#pragma once

#include <QtGlobal>

#ifdef BUILD_YY_COMMON_STATIC
#   define YY_COMMON_EXPORT
#else
#   ifdef BUILD_YY_COMMON
#       define YY_COMMON_EXPORT Q_DECL_EXPORT
#   else
#       define YY_COMMON_EXPORT Q_DECL_IMPORT
#   endif
#endif

#ifdef BUILD_YY_COMMON_RESOURCE_STATIC
#   define YY_COMMON_RESOURCE_EXPORT
#else
#   ifdef BUILD_YY_COMMON_RESOURCE
#       define YY_COMMON_RESOURCE_EXPORT Q_DECL_EXPORT
#   else
#       define YY_COMMON_RESOURCE_EXPORT Q_DECL_IMPORT
#   endif
#endif

#ifdef BUILD_YY_CHANNEL_RESOURCE_STATIC
#   define YY_CHANNEL_RESOURCE_EXPORT
#else
#   ifdef BUILD_YY_CHANNEL_RESOURCE
#       define YY_CHANNEL_RESOURCE_EXPORT Q_DECL_EXPORT
#   else
#       define YY_CHANNEL_RESOURCE_EXPORT Q_DECL_IMPORT
#   endif
#endif

#  ifdef BUILD_DW_ADDONFW
#	 define DW_ADDONFW_EXPORT	DW_DECL_EXPORT
#else
#	 define DW_ADDONFW_EXPORT	DW_DECL_IMPORT
#endif //BUILD_DW_ADDONFW

#  ifdef BUILD_YY_MAINFRAME
#    define YY_MAINFRAME_EXPORT Q_DECL_EXPORT
#  else
#    define YY_MAINFRAME_EXPORT Q_DECL_IMPORT
#  endif // BUILD_YY_MAINFRAME

#  ifdef BUILD_YY_OPEPLATFORM
#	 define YY_OPEPLATFORM_EXPORT	Q_DECL_EXPORT
#else
#	 define YY_OPEPLATFORM_EXPORT	Q_DECL_IMPORT
#endif //BUILD_YY_OPEPLATFORM

#  ifdef BUILD_YY_STATISTIC
#	 define YY_STATISTIC_EXPORT	Q_DECL_EXPORT
#else
#	 define YY_STATISTIC_EXPORT	Q_DECL_IMPORT
#endif //BUILD_YY_STATISTIC

// ����IM, group, channel, plugin,�����϶���Ӧ�õ����κ��ࡣ
// ��������QT�Ķ�̬����dll����dll��������ţ�����code coverage������ע�������dll�޷������أ�
// ���Զ���debug/release����qaccessibleyyspecialwidget.cpp����һЩ�࣬�����е�dll������̬���ء��ڷ��а���,DUI_ACCESSIBLE�Ķ��岻���ڣ�������Ȼ����ȫ��̬����.
// ���������޸ģ���֪ͨdw_zhuangxuhui
#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YY_IM
#		define YY_IM_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_IM_EXPORT	Q_DECL_IMPORT
#	endif // BUILD_YY_IM
#else
#	define YY_IM_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YY_GROUP
#		define YY_GROUP_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_GROUP_EXPORT	Q_DECL_IMPORT
#	endif 
#else
#	define YY_GROUP_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE  
#	ifdef BUILD_YY_CHANNEL
#		define YY_CHANNEL_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_CHANNEL_EXPORT	Q_DECL_IMPORT
#	endif 
#else
#	define YY_CHANNEL_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE  
#	ifdef BUILD_YY_PLUGIN
#		define YY_PLUGIN_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_PLUGIN_EXPORT	Q_DECL_IMPORT
#	endif 
#else
#	define YY_PLUGIN_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YYGAME_GROUP
#		define YYGAME_GROUP_EXPORT	Q_DECL_EXPORT
#	else
#		define YYGAME_GROUP_EXPORT	Q_DECL_IMPORT
#	endif 
#else
#	define YYGAME_GROUP_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YY_IMAPP
#		define YY_IMAPP_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_IMAPP_EXPORT	Q_DECL_IMPORT
#	endif // BUILD_YY_IMAPP
#else
#	define YY_IMAPP_EXPORT	
#endif

#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YY_WORLD
#		define YY_WOLRD_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_WOLRD_EXPORT	Q_DECL_IMPORT
#	endif // BUILD_YY_WORLD
#else
#	define YY_WOLRD_EXPORT 
#endif


#ifdef DUI_ACCESSIBLE   
#	ifdef BUILD_YY_CHANNEL_LOGIC
#		define YY_CHANNEL_LOGIC_EXPORT	Q_DECL_EXPORT
#	else
#		define YY_CHANNEL_LOGIC_EXPORT	Q_DECL_IMPORT
#	endif // BUILD_YY_WORLD
#else
#	define YY_CHANNEL_LOGIC_EXPORT 
#endif
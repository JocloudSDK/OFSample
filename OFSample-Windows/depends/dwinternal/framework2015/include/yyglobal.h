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

// 对于IM, group, channel, plugin,理论上都不应该导出任何类。
// 但是由于QT的动态加载dll会检测dll的特殊符号，导致code coverage测试中注入代码后的dll无法被加载，
// 所以对于debug/release版在qaccessibleyyspecialwidget.cpp导出一些类，让所有的dll不被静态加载。在发行版中,DUI_ACCESSIBLE的定义不存在，所以仍然会是全动态加载.
// 以下如有修改，请通知dw_zhuangxuhui
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
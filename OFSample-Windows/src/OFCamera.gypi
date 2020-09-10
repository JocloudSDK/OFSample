{
    "includes": ["common.gypi"],# this is need by any target
    "variables": {
    },
    "targets": [
        {
            "target_name": "OFCamera",
            "type": "executable",
            "sources": [
                #
                #imgui part
                #
                "Imgui/imconfig.h",
                "Imgui/imgui_internal.h",
                "Imgui/imstb_rectpack.h",
                "Imgui/imstb_textedit.h",
                "Imgui/imstb_truetype.h",
                "Imgui/imgui.h",
                "Imgui/imgui.cpp",
                "Imgui/imgui_draw.cpp",
                "Imgui/imgui_widgets.cpp",
                "Imgui/imgui_extension.h",
                "Imgui/imgui_extension.cpp",
                "Imgui/imgui_impl_glfw.h",
                "Imgui/imgui_impl_glfw.cpp",
                "Imgui/imgui_impl_opengl3.h",
                "Imgui/imgui_impl_opengl3.cpp",
                #
                #utils part
                #
                "utils/pathutils.h",
                "utils/pathutils.cpp",
                "utils/fileutils.h",
                "utils/fileutils.cpp",
                #
                #effectmanager
                #
                "effectmanager/effectmanager.h",
                "effectmanager/effectmanager.cpp",
                #
                #rapidjson
                #
                "rapidjson/rapidjson.h",
                "rapidjson/document.h",
                #
                #camera
                #
                "Camera.h",
                "Camera.cpp",
                #
                #orangefilter render
                #
                "OrangeFilterRender.h",
                "OrangeFilterRender.cpp",
                "OrangeHelper.h",
                "OrangeHelper.cpp",
                #
                #ui
                #
                "OrangeFilterUI.h",
                "OrangeFilterUI.cpp",
                "Texture.h",
                "Texture.cpp",
                "main.cpp",
            ],
            
            #'msvs_precompiled_header': 'precompiled.h',
            #'msvs_precompiled_source': 'precompiled.cpp',
            # Required so that the printf actually causes a build failure
            # if the pch isn't included.
            #'msvs_settings': {
            # 'VCCLCompilerTool': {
            #    'WarningLevel': '3',
            #   'WarnAsError': 'true',
            #  },
            # },
            
            "include_dirs": [
                "imgui",
                "utils",
                "utils/log",
                "effectmanager",
                "rapidjson",
                "crashreport",
                "../depends/dwinternal/orangefilterpub2013/include",
                "../depends/dwinternal/framework2015/include",
                #"../ThridParty/DirectShow/Include", #using this include may lead to compile error because of basetsd.h first find in this dir
                "../ThridParty/FreeImage/include",
                "../ThridParty/glfw/include",
                "../ThridParty/opencv400/opencv/build/include",
                "../ThridParty/curl/include",
                "../ThridParty/zlib/include",
				"../ThridParty/CrashReport/include",
            ],
            "library_dirs": [
                "../depends/dwinternal/orangefilterpub2013/lib/Release/x86",
                "../ThridParty/DirectShow/Lib/x86",
                "../ThridParty/FreeImage/lib/x86",
                "../ThridParty/glfw/lib-vc2010-32",
                "../ThridParty/opencv400/opencv/build/x86/vc14/lib",
                "../ThridParty/curl/lib",
                "../ThridParty/zlib/lib/x86",
				"../ThridParty/CrashReport/lib",
                "../depends/dwinternal/framework2015/lib/release",
                "<(PRODUCT_DIR)",
            ],
            
            "conditions": [
                [
                    'OS=="win"',
                    {
                        "defines": [
                            "WIN32",
                        ],
                        "dependencies": [
                            #"CrashReport",
                            #"BugReport",
                        ],
                        "configurations": {
                            "Debug": {
                                "msvs_settings": {
                                    "VCCLCompilerTool": {"RuntimeLibrary": "3"},  # /MDd
                                    "VCLinkerTool": {
                                        "ImageHasSafeExceptionHandlers": "false",  # /SAFESEH:NO
                                        "AdditionalDependencies": 
                                        [
                                            "opengl32.lib",
                                            "glfw3.lib",
                                            "glew32.lib",
                                            "of_effect.lib",
                                            "opencv_world400d.lib",
                                            "FreeImage.lib",
                                            "libcurl.lib",
                                            "dwbase.lib",
                                            "zlib.lib",
											"CrashReport.lib",
                                            "%(AdditionalDependencies)", #Inherited system default libs
                                        ],
                                        "OutputFile": "$(MSBuildProjectDirectory)/../bin/$(ConfigurationName)/$(ProjectName).exe",
                                        "ProgramDatabaseFile": "$(MSBuildProjectDirectory)/../bin/$(ConfigurationName)/$(TargetName).pdb",
                                        "ImportLibrary": "$(MSBuildProjectDirectory)/../lib/$(ConfigurationName)/$(TargetName).lib",
                                    },
                                },  # msvs_settings
                            },  # Debug
                            "Release": {
                                "msvs_settings": {
                                    "VCCLCompilerTool": {"RuntimeLibrary": "2"},  # /MD
                                    "VCLinkerTool": {
                                        "ImageHasSafeExceptionHandlers": "false",  # /SAFESEH:NO
                                        "AdditionalDependencies": 
                                        [
                                            "opengl32.lib",
                                            "glfw3.lib",
                                            "glew32.lib",
                                            "of_effect.lib",
                                            "opencv_world400.lib",
                                            "FreeImage.lib",
                                            "libcurl.lib",
                                            "dwbase.lib",
                                            "zlib.lib",
											"CrashReport.lib",
                                            "%(AdditionalDependencies)", #Inherited system default libs
                                        ],
                                        "OutputFile": "$(MSBuildProjectDirectory)/../bin/$(ConfigurationName)/$(ProjectName).exe",
                                        "ProgramDatabaseFile": "$(MSBuildProjectDirectory)/../bin/$(ConfigurationName)/$(TargetName).pdb",
                                        "ImportLibrary": "$(MSBuildProjectDirectory)/../lib/$(ConfigurationName)/$(TargetName).lib",
                                    },
                                },  # msvs_settings
                            },  # Release
                        },  # configurations
                    },
                ],
                [
                    'OS=="mac"',
                    {
                        #'mac_bundle': 1,
                        "link_settings": {
                            "libraries": [
                            
                            ]
                        },
                        "mac_framework_dirs": [
                            "$(SDKROOT)/System/Library/Frameworks",  # probably the system path is used by default
                        ],
                        "xcode_settings": {
                            "INFOPLIST_FILE": "src/OFCamera/Info.plist",
                            "OTHER_LDFLAGS": [
                                "-headerpad_max_install_names",
                                "-Wl,-rpath,<(QTDIR)/lib",
                                "-F<(QTDIR)/lib",
                            ],
                        },
                    },
                ],
            ],
        },  # target OFCamera.
        {
            "target_name": "run_deploy",
            "type": "none",
            "dependencies": ["OFCamera"],
            "conditions": [
                [
                    'OS=="win"',
                    {
                    	"msvs_postbuild": r'echo off & echo %BUILD_DISPLAY_NAME% > package.ini & echo %BUILD_DISPLAY_NAME% > ../bin/$(Configuration)/package.ini',
                    		
                        "copies": [
                            {
                                "destination": "../bin/$(ConfigurationName)",
                                "files": [
                                    "../depends/dwinternal/framework2015/bin/release/dwbase.dll",
                                    "../depends/dwinternal/framework2015/bin/release/dwutility.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/msvcp120.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/msvcr120.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/of_effect.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/glew32.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_cnn.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_core.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_kits.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_new_gesture.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_facedetection.dll",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/vn_seg.dll",
									"../depends/dwinternal/orangefilterpub2013/bin/release/x86/venus.dll",
                                    "../ThridParty/FreeImage/bin/x86/FreeImage.dll",
                                    "../ThridParty/opencv400/opencv/build/x86/vc14/bin/opencv_world400d.dll",
                                    "../ThridParty/opencv400/opencv/build/x86/vc14/bin/opencv_world400.dll",
                                    "../ThridParty/curl/bin/libcurl.dll",
                                    "../ThridParty/curl/bin/nghttp2.dll",
                                    "../ThridParty/zlib/bin/x86/zlib.dll",
									"../ThridParty/CrashReport/bin/crashreport.dll",
									"../ThridParty/CrashReport/bin/BugReport.exe",
                                    "../ThridParty/curl/certificate/cacert-2020-01-01.pem",
                                    "../orangefilter/effects/",
                                ],
                            },
                            {
                                "destination": "../bin/$(ConfigurationName)/res",
                                "files": [
                                    "../res/msyh.ttc",
                                    "../res/icon_Movebutton_nor.png",
                                    "../res/selected.png",
                                    "../res/title.png",
                                ],
                            },
                            {
                                "destination": "../bin/$(ConfigurationName)/venus_models/face",
                                "files": [
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Face_Key_Points_0.vnmodel",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Face_Key_Points_1.vnmodel",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Face_Key_Points_2.vnmodel",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Face_Key_Points_3.vnmodel",
									"../depends/dwinternal/orangefilterpub2013/bin/release/x86/Face_Key_Points_4.vnmodel",
                                ],
                            },
                            {
                                "destination": "../bin/$(ConfigurationName)/venus_models/gesture",
                                "files": [
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Gesture_Recognition_0.vnmodel",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Gesture_Recognition_1.vnmodel",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Gesture_Recognition_2.vnmodel",
                                ],
                            },
                            {
                                "destination": "../bin/$(ConfigurationName)/venus_models/segment",
                                "files": [
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Background_Segmentation_0.dat",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Background_Segmentation_1.dat",
                                    "../depends/dwinternal/orangefilterpub2013/bin/release/x86/Background_Segmentation_2.dat",
                                ],
                            },

                        ],
                    },
                ],
                [
                    'OS=="mac"',
                    {
                        "copies": [
                            {
                                "destination": "<(PRODUCT_DIR)",
                                "files": [
                                    
                                ],
                            },
                        ]
                    },
                ],
            ],
        },
    ],
}

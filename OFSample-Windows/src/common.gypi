{
    "target_defaults": {
        "conditions": [
            [
                'OS=="win"',
                {
                    "defines": [
                        "WIN32",
                        
                    ],
                    "msvs_configuration_attributes": {"CharacterSet": "1"},
                    "msbuild_toolset": "v140_xp",
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            #'WarningLevel': '4',
                            "WarnAsError": "true",
                            "DisableSpecificWarnings": ["4819"],
                            "Detect64BitPortabilityProblems": "true",
                        }
                    },
                },
            ],
            
            [
                'OS=="mac"',
                {
                    "defines": [
                        "__unix__",
                        "_MACOS",
                    ],
                    "cflags": ["-Wall"],
                },
            ],
        ],
        "configurations": {
            "Debug": {
                "defines": ["_DEBUG"],
                "conditions": [
                    [
                        'OS=="win"',
                        {
                            "msvs_settings": {
                                "VCCLCompilerTool": {
                                    "Optimization": "0",
                                    "MinimalRebuild": "true",
                                    "BasicRuntimeChecks": "3",
                                    "DebugInformationFormat": "4",
                                    "conditions": [
                                        [
                                            'library=="shared_library"',
                                            {"RuntimeLibrary": "3"},  # /MDd
                                            {"RuntimeLibrary": "1"},  # /MTd
                                        ]
                                    ],
                                },
                                "VCLinkerTool": {
                                    "GenerateDebugInformation": "true",
                                    "LinkIncremental": "2",
                                },
                            }
                        },
                    ],
                    [
                        'OS=="mac"',
                        {
                            "xcode_settings": {
                                "GCC_OPTIMIZATION_LEVEL": "0",
                                "GCC_GENERATE_DEBUGGING_SYMBOLS": "YES",
                                "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
                            }
                        },
                    ],
                ],
            },
            "Release": {
                "conditions": [
                    [
                        'OS=="win"',
                        {
                            "msvs_settings": {
                                "VCCLCompilerTool": {
                                    "Optimization": "2",
                                    "conditions": [
                                        [
                                            'library=="shared_library"',
                                            {"RuntimeLibrary": "2"},  # /MD
                                            {"RuntimeLibrary": "0"},  # /MT
                                        ]
                                    ],
                                },
                                "VCLinkerTool": {
                                    "GenerateDebugInformation": "true",
                                    "LinkIncremental": "2",
                                },
                            }
                        },
                    ],
                    [
                        'OS=="mac"',
                        {
                            "xcode_settings": {
                                "GCC_GENERATE_DEBUGGING_SYMBOLS": "NO",
                                "GCC_OPTIMIZATION_LEVEL": "3",
                                "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
                                # -fstrict-aliasing. Mainline gcc enables
                                # this at -O2 and above, but Apple gcc does
                                # not unless it is specified explicitly.
                                "GCC_STRICT_ALIASING": "YES",
                            }
                        },
                    ],
                ]
            },
        },
    }
}

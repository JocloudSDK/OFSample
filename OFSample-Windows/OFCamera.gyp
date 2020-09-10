{
    "target_defaults": {
        "includes": ["src/common.gypi"],
        "default_configuration": "Release",
    },
    "conditions": [
        [
            'OS=="win"',
            {
                "includes": [
                    "src/OFCamera.gypi",
                ],
            },
        ],
        [
            'OS=="mac"',
            {
                "includes": [
                    "src/OFCamera.gypi",
                ]
            },
        ],
    ],
}

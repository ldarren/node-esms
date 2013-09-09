{
    "targets": [
        {
            "target_name": "roster_creator",
            "sources": [ "src/roster_creator_v8.cpp", "src/object_v8.cpp", "src/util.cpp" ]
        },
        {
            "target_name": "fixtures",
            "sources": [ "src/fixtures_v8.cpp" ]
        },
        {
            "target_name": "tsc",
            "sources": [ "src/tsc_v8.cpp", "src/object_v8.cpp", "src/util.cpp" ]
        },
        {
            "target_name": "esms",
            "sources": [ 
                "src/esms_v8.cpp", 
                "src/report_event.cpp", 
                "src/cond.cpp", 
                "src/cond_action.cpp", 
                "src/cond_condition.cpp", 
                "src/mt.cpp", 
                "src/cond_utils.cpp", 
                "src/comment_v8.cpp", 
                "src/penalty_v8.cpp", 
                "src/tactics_v8.cpp", 
                "src/object_v8.cpp", 
                "src/util.cpp" ]
        },
        {
            "target_name": "updtr",
            "sources": [ 
                "src/updtr_v8.cpp", 
                "src/league_table_v8.cpp",
                "src/object_v8.cpp", 
                "src/util.cpp" ]
        }
    ]
}
